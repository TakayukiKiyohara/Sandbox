/*!
 *@brief	スキンモデルデータ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"

int refCount = 0;
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { refCount = (p)->Release(); (p)=NULL; } }

#endif

namespace {
	using namespace tkEngine;
	//--------------------------------------------------------------------------------------
	// update the frame matrices
	//--------------------------------------------------------------------------------------
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix)
	{
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

		if (pParentMatrix != NULL)
			D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
		else
			pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

		if (pFrame->pFrameSibling != NULL)
		{
			UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
		}
	}

	void InnerDestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		UINT iMaterial;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		SAFE_DELETE_ARRAY(pMeshContainer->pAttributeTable);
		SAFE_DELETE_ARRAY(pMeshContainer->Name);
		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

		// release all the allocated textures
		if (pMeshContainer->ppTextures != NULL)
		{
			for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
			{
				SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
			}
		}

		SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
		SAFE_DELETE_ARRAY(pMeshContainer->materials);
		SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
		SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pSkinInfo);
		SAFE_RELEASE(pMeshContainer->pOrigMesh);
		SAFE_DELETE(pMeshContainer);
	}
	HRESULT GenerateSkinnedMesh(
		IDirect3DDevice9* pd3dDevice, 
		D3DXMESHCONTAINER_DERIVED* pMeshContainer
	)
	{
		HRESULT hr = S_OK;
		D3DCAPS9 d3dCaps;
		pd3dDevice->GetDeviceCaps(&d3dCaps);
		
		if (pMeshContainer->pSkinInfo == NULL)
			return hr;
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
		
		{
			// Get palette size
			// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
			// (96 - 9) /3 i.e. Maximum constant count - used constants 
			UINT MaxMatrices = 26;
			pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

			DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;
			
			SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
			hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
				(
					pMeshContainer->pOrigMesh,
					Flags,
					pMeshContainer->NumPaletteEntries,
					pMeshContainer->pAdjacency,
					NULL, NULL, NULL,
					&pMeshContainer->NumInfl,
					&pMeshContainer->NumAttributeGroups,
					&pMeshContainer->pBoneCombinationBuf,
					&pMeshContainer->MeshData.pMesh);
			if (FAILED(hr))
				goto e_Exit;
				
		
			
			// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
			DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
				D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
			if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
			{
				LPD3DXMESH pMesh;
				hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
					pd3dDevice, &pMesh);
				if (!FAILED(hr))
				{
					SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
					pMeshContainer->MeshData.pMesh = pMesh;
					pMesh = NULL;
				}
			}

			D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
			LPD3DVERTEXELEMENT9 pDeclCur;
			hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
			if (FAILED(hr))
				goto e_Exit;
			
			// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
			//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
			//          this is more of a "cast" operation
			pDeclCur = pDecl;
			while (pDeclCur->Stream != 0xff)
			{
				if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
					pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
				pDeclCur++;
			}

			hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
			if (FAILED(hr))
				goto e_Exit;

		}
	e_Exit:
		return hr;
	}
	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName)
	{
		UINT cbLength;

		if (Name != NULL)
		{
			cbLength = (UINT)strlen(Name) + 1;
			*pNewName = new CHAR[cbLength];
			if (*pNewName == NULL)
				return E_OUTOFMEMORY;
			memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
		}
		else
		{
			*pNewName = NULL;
		}

		return S_OK;
	}
	//--------------------------------------------------------------------------------------
	// Called to setup the pointers for a given bone to its transformation matrix
	//--------------------------------------------------------------------------------------
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME rootFrame)
	{
		UINT iBone, cBones;
		D3DXFRAME_DERIVED* pFrame;

		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL)
		{
			cBones = pMeshContainer->pSkinInfo->GetNumBones();

			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones] ;
			if (pMeshContainer->ppBoneMatrixPtrs == NULL)
				return E_OUTOFMEMORY;

			for (iBone = 0; iBone < cBones; iBone++)
			{
				pMeshContainer->ppBoneMatrixPtrs[iBone] = NULL;
				LPCSTR boneName = pMeshContainer->pSkinInfo->GetBoneName(iBone);
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame,
					boneName);
				if (pFrame == NULL)
					return E_FAIL;

				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
		return S_OK;
	}


	

	class CAllocateHierarchy : public ID3DXAllocateHierarchy
	{
	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			CONST D3DXMESHDATA *pMeshData,
			CONST D3DXMATERIAL *pMaterials,
			CONST D3DXEFFECTINSTANCE *pEffectInstances,
			DWORD NumMaterials,
			CONST DWORD *pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER *ppNewMeshContainer);
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

		CAllocateHierarchy(CSkinModelData* data) :
			m_skinModelData(data)
		{
		}
	private:
		CSkinModelData* m_skinModelData;
	};
	//--------------------------------------------------------------------------------------
	// Name: CAllocateHierarchy::CreateFrame()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
	{
		HRESULT hr = S_OK;
		D3DXFRAME_DERIVED* pFrame;

		*ppNewFrame = NULL;

		pFrame = new D3DXFRAME_DERIVED;
		if (pFrame == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = AllocateName(Name, &pFrame->Name);
		if (FAILED(hr))
			goto e_Exit;
		// initialize other data members of the frame
		D3DXMatrixIdentity(&pFrame->TransformationMatrix);
		D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

		pFrame->pMeshContainer = NULL;
		pFrame->pFrameSibling = NULL;
		pFrame->pFrameFirstChild = NULL;

		*ppNewFrame = pFrame;
		pFrame = NULL;

	e_Exit:
		delete pFrame;
		return hr;
	}
	//--------------------------------------------------------------------------------------
	// Name: CAllocateHierarchy::CreateMeshContainer()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT CAllocateHierarchy::CreateMeshContainer(
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer)
	{
		
		HRESULT hr = S_OK;
		D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
		UINT NumFaces;
		UINT iMaterial;
		UINT iBone, cBones;
		LPDIRECT3DDEVICE9 pd3dDevice = NULL;

		LPD3DXMESH pMesh = NULL;

		*ppNewMeshContainer = NULL;

		// this sample does not handle patch meshes, so fail when one is found
		if (pMeshData->Type != D3DXMESHTYPE_MESH)
		{
			hr = E_FAIL;
			goto e_Exit;
		}

		// get the pMesh interface pointer out of the mesh data structure
		pMesh = pMeshData->pMesh;
		
		DWORD numVert = pMesh->GetNumVertices();
		// this sample does not FVF compatible meshes, so fail when one is found
		if (pMesh->GetFVF() == 0)
		{
			hr = E_FAIL;
			goto e_Exit;
		}

		// allocate the overloaded structure to return as a D3DXMESHCONTAINER
		pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
		if (pMeshContainer == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
		memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

		// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
		hr = AllocateName(Name, &pMeshContainer->Name);
		if (FAILED(hr))
			goto e_Exit;

		pMesh->GetDevice(&pd3dDevice);
		NumFaces = pMesh->GetNumFaces();

		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();

		D3DVERTEXELEMENT9 decl[] = {
			{ 0, 0 ,	D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION		, 0 },
			{ 0, 16,    D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT	, 0 },
			{ 0, 32,    D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES	, 0 },
			{ 0, 48,	D3DDECLTYPE_FLOAT3		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL		, 0 },
			{ 0, 60,	D3DDECLTYPE_FLOAT3		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT		, 0 },
			{ 0, 72,	D3DDECLTYPE_FLOAT2		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD		, 0 },
			D3DDECL_END()
		};
		

		// allocate memory to contain the material information.  This sample uses
		//   the D3D9 materials and texture names instead of the EffectInstance style materials
		pMeshContainer->NumMaterials = max(1, NumMaterials);
		pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
		pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
		pMeshContainer->materials = new CSkinModelMaterial[pMeshContainer->NumMaterials];
		pMeshContainer->textures = new CTexture[pMeshContainer->NumMaterials];
		pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
		if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
		memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

		// if materials provided, copy them
		if (NumMaterials > 0)
		{
			memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

			for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
			{
				if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
				{
					char* baseDir = "Assets/modelData/";
					char filePath[64];
					strcpy(filePath, baseDir);
					strcat(filePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					if (FAILED(D3DXCreateTextureFromFile(
						pd3dDevice, 
						filePath,
						&pMeshContainer->ppTextures[iMaterial]))
					) {
						pMeshContainer->ppTextures[iMaterial] = NULL;
					}

					//マテリアルを生成。
					//マテリアル名はディフューズテクスチャの名前。
					pMeshContainer->materials[iMaterial].SetMaterialName(pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					pMeshContainer->textures[iMaterial].SetTextureDX(pMeshContainer->ppTextures[iMaterial]);
					pMeshContainer->materials[iMaterial].SetTexture("g_diffuseTexture", &pMeshContainer->textures[iMaterial]);
					// don't remember a pointer into the dynamic memory, just forget the name after loading
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
					m_skinModelData->AddSkinModelMaterial(&pMeshContainer->materials[iMaterial]);
				}
			}
		}
		else // if no materials provided, use a default one
		{
			pMeshContainer->pMaterials[0].pTextureFilename = NULL;
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}
		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();
		// if there is skinning information, save off the required data and then setup for HW skinning
		if (pSkinInfo != NULL)
		{
			// first save off the SkinInfo and original mesh data
			pMeshContainer->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
			cBones = pSkinInfo->GetNumBones();
			pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
			if (pMeshContainer->pBoneOffsetMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}

			// get each of the bone offset matrices so that we don't need to get them later
			for (iBone = 0; iBone < cBones; iBone++)
			{
				pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
			}
			
			// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
			hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
	
			if (FAILED(hr))
				goto e_Exit;
			
			LPD3DXMESH pOutMesh, pTmpMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMesh(
				pMeshContainer->MeshData.pMesh->GetOptions(),
				decl,
				pd3dDevice, &pOutMesh);
			
			if (FAILED(hr))
				goto e_Exit;
			
			//一時メッシュに退避。
			pTmpMesh = pOutMesh;
			//D3DXComputeTangentFrameExを実行すると属性テーブルの情報が失われる・・・。
			DWORD numAttributeTable;
			pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &numAttributeTable);
			pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[numAttributeTable];
			pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
			hr = D3DXComputeTangentFrameEx(
				pTmpMesh,
				D3DDECLUSAGE_TEXCOORD,
				0,
				D3DDECLUSAGE_TANGENT,
				0,
				D3DX_DEFAULT,
				0,
				D3DDECLUSAGE_NORMAL,
				0,
				0,
				NULL,
				0.01f,    //ボケ具合.値をおおきくするとぼけなくなる
				0.25f,
				0.01f,
				&pOutMesh,
				NULL
				);
			//一時メッシュを破棄。
			SAFE_RELEASE(pTmpMesh);
			SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
			pMeshContainer->MeshData.pMesh = pOutMesh;
			
			if (FAILED(hr))
				goto e_Exit;
		}
		else {
			LPD3DXMESH pOutMesh, pTmpMesh;
			DWORD numVert = pMeshContainer->MeshData.pMesh->GetNumVertices();
			hr = pMeshContainer->MeshData.pMesh->CloneMesh(
				pMeshContainer->MeshData.pMesh->GetOptions(),
				decl,
				pd3dDevice, &pOutMesh);
			DWORD numAttributeTable;
			pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &numAttributeTable);
			pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[numAttributeTable];
			pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
			numVert = pMeshContainer->MeshData.pMesh->GetNumVertices();
			
			//一時メッシュに退避。
			pTmpMesh = pOutMesh;

			hr = D3DXComputeTangentFrameEx(
				pTmpMesh,
				D3DDECLUSAGE_TEXCOORD,
				0,
				D3DDECLUSAGE_TANGENT,
				0,
				D3DX_DEFAULT,
				0,
				D3DDECLUSAGE_NORMAL,
				0,
				0,
				NULL,
				0.01f,    //ボケ具合.値をおおきくするとぼけなくなる
				0.25f,
				0.01f,
				&pOutMesh,
				NULL
				);
			//一時メッシュを破棄。
			SAFE_RELEASE(pTmpMesh);
			numVert = pOutMesh->GetNumVertices();
			SAFE_RELEASE(pMesh);
			pMeshContainer->MeshData.pMesh = pOutMesh;
			if (FAILED(hr))
				goto e_Exit;

		}

		*ppNewMeshContainer = pMeshContainer;
		
		pMeshContainer = NULL;

	e_Exit:
		
		SAFE_RELEASE(pd3dDevice);

		// call Destroy function to properly clean up the memory allocated 
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}

		return hr;
	}
	//--------------------------------------------------------------------------------------
	// Name: CAllocateHierarchy::DestroyFrame()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
	{
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
		return S_OK;
	}




	//--------------------------------------------------------------------------------------
	// Name: CAllocateHierarchy::DestroyMeshContainer()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		InnerDestroyMeshContainer(pMeshContainerBase);
		return S_OK;
	}


}
namespace tkEngine{
	CSkinModelData::CSkinModelData() :
		m_frameRoot(nullptr),
		m_isClone(false),
		m_animController(nullptr),
		m_vertexDeclForInstancingDraw(nullptr),
		m_numInstance(0),
		m_vertexBufferStride(0)
	{
	}
	CSkinModelData::~CSkinModelData()
	{
		Release();
	}
	void CSkinModelData::Release()
	{
		if(m_vertexDeclForInstancingDraw){
			SAFE_RELEASE(m_vertexDeclForInstancingDraw);
		}
		if (m_isClone && m_frameRoot) {
			//クローン
			DeleteCloneSkeleton(m_frameRoot);
			m_frameRoot = nullptr;
		}
		else {
			//オリジナル。
			DeleteSkeleton(m_frameRoot);
		}
		m_instanceVertexBuffer.Release();
		m_numInstance = 0;
	}
	
	void CSkinModelData::DeleteSkeleton(LPD3DXFRAME frame)
	{
		if (!frame) {
			return;
		}
		if (frame->pMeshContainer != NULL)
		{
			//メッシュコンテナがある。
			InnerDestroyMeshContainer(frame->pMeshContainer);
		}

		if (frame->pFrameSibling != NULL)
		{
			//兄弟がいる。
			DeleteSkeleton(frame->pFrameSibling);
		}

		if (frame->pFrameFirstChild != NULL)
		{
			//子供がいる。
			DeleteSkeleton(frame->pFrameFirstChild);
		}
		SAFE_DELETE_ARRAY(frame->Name);
		SAFE_DELETE(frame);
	}
	//--------------------------------------------------------------------------------------
	// Called to setup the pointers for a given bone to its transformation matrix
	//--------------------------------------------------------------------------------------
	HRESULT CSkinModelData::SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame)
	{
		HRESULT hr;

		if (pFrame->pMeshContainer != NULL)
		{
			//頂点ストライドもここで調べる。
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			pFrame->pMeshContainer->MeshData.pMesh->GetDeclaration(Declaration);
			m_vertexBufferStride = D3DXGetDeclVertexSize(Declaration, 0);
			hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pRootFrame);
			if (FAILED(hr))
				return hr;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			hr = SetupBoneMatrixPointers(pFrame->pFrameSibling, pRootFrame);
			if (FAILED(hr))
				return hr;
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild, pRootFrame);
			if (FAILED(hr))
				return hr;
		}

		return S_OK;
	}
	void CSkinModelData::SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr)
	{
		if (animCtr == nullptr) {
			return;
		}
		HRESULT hr = animCtr->RegisterAnimationOutput(frame->Name, &frame->TransformationMatrix, nullptr, nullptr, nullptr);
		if (frame->pFrameSibling != nullptr) {
			SetupOutputAnimationRegist(frame->pFrameSibling, animCtr);
		}
		if (frame->pFrameFirstChild != nullptr)
		{
			SetupOutputAnimationRegist(frame->pFrameFirstChild, animCtr);
		}
	}
	void CSkinModelData::LoadModelData( const char* filePath, CAnimation* anim )
	{	
		CAllocateHierarchy alloc(this);
		HRESULT hr = D3DXLoadMeshHierarchyFromX(
			filePath,
			D3DXMESH_VB_MANAGED,
			Engine().GetD3DDevice(),
			&alloc,
			nullptr,
			&m_frameRoot,
			&m_animController
		);
		
		TK_ASSERT(SUCCEEDED(hr), "Failed D3DXLoadMeshHierarchyFromX");
		SetupBoneMatrixPointers(m_frameRoot, m_frameRoot);
		if (anim && m_animController) {
			anim->Init(m_animController);
		}
		else {
			SAFE_RELEASE(m_animController);
		}
	}
	
	void CSkinModelData::CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame)
	{
		//名前と行列をコピー。
		dstFrame->TransformationMatrix = srcFrame->TransformationMatrix;
		//メッシュコンテナをコピー。メッシュは使いまわす。
		if (srcFrame->pMeshContainer) {
			dstFrame->pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
			memcpy(dstFrame->pMeshContainer, srcFrame->pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));
		}
		else {
			dstFrame->pMeshContainer = NULL;
		}
		AllocateName(srcFrame->Name, &dstFrame->Name);
	

		if (srcFrame->pFrameSibling != nullptr) {
			//兄弟がいるので、兄弟のためのメモリを確保。
			dstFrame->pFrameSibling = new D3DXFRAME_DERIVED;
			dstFrame->pFrameSibling->pFrameFirstChild = nullptr;
			dstFrame->pFrameSibling->pFrameSibling = nullptr;
			dstFrame->pFrameSibling->pMeshContainer = nullptr;
			CloneSkeleton(dstFrame->pFrameSibling, srcFrame->pFrameSibling);
		}
		if (srcFrame->pFrameFirstChild != nullptr)
		{
			//子供がいるので、子供のためのメモリを確保。
			dstFrame->pFrameFirstChild = new D3DXFRAME_DERIVED;
			dstFrame->pFrameFirstChild->pFrameFirstChild = nullptr;
			dstFrame->pFrameFirstChild->pFrameSibling = nullptr;
			dstFrame->pFrameFirstChild->pMeshContainer = nullptr;

			CloneSkeleton(dstFrame->pFrameFirstChild, srcFrame->pFrameFirstChild);
		}
	}
	void CSkinModelData::DeleteCloneSkeleton(LPD3DXFRAME frame)
	{
		
		if (frame->pFrameSibling != nullptr) {
			//兄弟
			DeleteCloneSkeleton(frame->pFrameSibling);
		}
		if (frame->pFrameFirstChild != nullptr)
		{
			//子供。
			DeleteCloneSkeleton(frame->pFrameFirstChild);
		}
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)(frame->pMeshContainer);
		if (pMeshContainer) {
			SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
			SAFE_DELETE(pMeshContainer);
		}
		SAFE_DELETE_ARRAY(frame->Name);
		SAFE_DELETE(frame);
	}
	void CSkinModelData::CloneModelData(const CSkinModelData& modelData, CAnimation* anim)
	{
		//スケルトンの複製を作成。。
		m_isClone = true;
		m_frameRoot = new D3DXFRAME_DERIVED;
		m_frameRoot->pFrameFirstChild = nullptr;
		m_frameRoot->pFrameSibling = nullptr;
		m_frameRoot->pMeshContainer = nullptr;
		CloneSkeleton(m_frameRoot, modelData.m_frameRoot);
		//アニメーションコントローラを作成して、スケルトンと関連付けを行う。
		if (modelData.m_animController) {
			modelData.m_animController->CloneAnimationController(
				modelData.m_animController->GetMaxNumAnimationOutputs(),
				modelData.m_animController->GetMaxNumAnimationSets(),
				modelData.m_animController->GetMaxNumTracks(),
				modelData.m_animController->GetMaxNumEvents(),
				&m_animController
				);
		
			SetupOutputAnimationRegist(m_frameRoot, m_animController);

			if (anim && m_animController) {
				anim->Init(m_animController);
			}
		}
		SetupBoneMatrixPointers(m_frameRoot, m_frameRoot);
	}
	void CSkinModelData::UpdateBoneMatrix(const CMatrix& matWorld)
	{
		UpdateFrameMatrices(m_frameRoot, r_cast<const D3DXMATRIX*>(&matWorld));
	}
	void CSkinModelData::CreateInstancingDrawData( int numInstance, SVertexElement* vertexElement )
	{
		m_numInstance = numInstance;
		CreateInstancingDrawData(m_frameRoot, numInstance, vertexElement);
	}
	bool CSkinModelData::CreateInstancingDrawData( LPD3DXFRAME frame, int numInstance, SVertexElement* vertexElement )
	{
		if(frame->pMeshContainer){
			
			//メッシュを発見。
			//まずはストリーム１の頂点バッファを追加した頂点定義を作成する。
			//頂点定義は全てのメッシュで同じ。
			D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
			frame->pMeshContainer->MeshData.pMesh->GetDeclaration(declElement);
			int elementIndex = 0;
			while(true){
				if(declElement[elementIndex].Type == D3DDECLTYPE_UNUSED){
					//もともとの頂点定義の終端を発見。
					//ここからインスタンシング用の頂点レイアウトを埋め込む。
					for(int i = 0; vertexElement[i].Type != D3DDECLTYPE_UNUSED; i++){
						declElement[elementIndex] = vertexElement[i];
						elementIndex++;
					}
					//終端を埋め込んで終わり。
					declElement[elementIndex] = D3DDECL_END();
					break;
				}
				elementIndex++;
			}
			//頂点定義の作成。
			LPDIRECT3DDEVICE9 pd3dDevice;
			frame->pMeshContainer->MeshData.pMesh->GetDevice(&pd3dDevice);
			pd3dDevice->CreateVertexDeclaration(declElement, &m_vertexDeclForInstancingDraw);
			SAFE_RELEASE(pd3dDevice);
			//頂点バッファの作成。
			
			DWORD vertexBufferStride = D3DXGetDeclVertexSize(vertexElement, 1);
			m_instanceVertexBuffer.Create(numInstance, vertexBufferStride, vertexElement, nullptr);
			return true;
		}
		if(frame->pFrameSibling != nullptr){
			//兄弟がいる
			if( CreateInstancingDrawData(frame->pFrameSibling, numInstance, vertexElement) ){
				return true;
			}
		}
		if(frame->pFrameFirstChild != nullptr){
			//子供がいる。
			if( CreateInstancingDrawData(frame->pFrameFirstChild, numInstance, vertexElement) ){
				return true;
			}
		}
		return false;
	}
	LPD3DXMESH CSkinModelData::GetOrgMesh(LPD3DXFRAME frame) const
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)(frame->pMeshContainer);
		if (pMeshContainer != nullptr) {
			return pMeshContainer->pOrigMesh;
		}
		if (frame->pFrameSibling != nullptr) {
			//兄弟
			LPD3DXMESH mesh = GetOrgMesh(frame->pFrameSibling);
			
			if (mesh) {
				return mesh;
			}
		}
		if (frame->pFrameFirstChild != nullptr)
		{
			//子供。
			LPD3DXMESH mesh = GetOrgMesh(frame->pFrameFirstChild);
			if (mesh) {
				return mesh;
			}
		}
		
		return nullptr;
	}
	LPD3DXMESH CSkinModelData::GetOrgMeshFirst() const
	{
		return GetOrgMesh(m_frameRoot);
	}
	
	CMatrix* CSkinModelData::FindBoneWorldMatrix(const char* boneName) 
	{
		return FindBoneWorldMatrix(boneName, m_frameRoot);
	}
	
	CMatrix* CSkinModelData::FindBoneWorldMatrix(const char* boneName, LPD3DXFRAME frame) 
	{
		if (frame->Name != NULL && strcmp(frame->Name, boneName) == 0) {
			//見つかった。
			D3DXFRAME_DERIVED* frameDer = (D3DXFRAME_DERIVED*)frame;
			return (CMatrix*)&frameDer->CombinedTransformationMatrix;
		}
		if (frame->pFrameSibling != nullptr) {
			//兄弟
			CMatrix* result = FindBoneWorldMatrix(boneName, frame->pFrameSibling);
			if (result != nullptr) {
				return result;
			}
		}
		if (frame->pFrameFirstChild != nullptr)
		{
			//子供。
			CMatrix* result = FindBoneWorldMatrix(boneName, frame->pFrameFirstChild);
			if (result != nullptr) {
				return result;
			}
		}
		return nullptr;
	}
	CSkinModelMaterial* CSkinModelData::FindMaterial(const char* matName)
	{
		for (CSkinModelMaterial* mat : m_materials) {
			if (strcmp(mat->GetMaterialName(), matName) == 0) {
				return mat;
			}
		}
		return nullptr;
	}
}