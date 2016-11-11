#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/prerender/tkShadowMap.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"

namespace tkEngine{
	
	void CSkinModel::DrawMeshContainer_InstancingDrawCommon(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* meshContainer, int materialID)
	{
		LPDIRECT3DVERTEXBUFFER9 vb;
		LPDIRECT3DINDEXBUFFER9 ib;
		LPD3DXMESH mesh = meshContainer->MeshData.pMesh;
		mesh->GetVertexBuffer(&vb);
		mesh->GetIndexBuffer(&ib);
		DWORD attributeTableSize = 256;
		
		DWORD stride = m_skinModelData->GetVertexBufferStride();

		pd3dDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_skinModelData->GetNumInstance());
		pd3dDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		
		pd3dDevice->SetVertexDeclaration(m_skinModelData->GetVertexDeclForInstancingDraw());

		pd3dDevice->SetStreamSource(0, vb, 0, stride);							//頂点バッファをストリーム0番目に設定
																					//ワールド行列を頂点バッファにコピー。
		
		pd3dDevice->SetStreamSource(
			1, 
			m_skinModelData->GetInstancingVertexBuffer().GetBody(), 
			0, 
			m_skinModelData->GetInstancingVertexBuffer().GetStride()
		);	
		pd3dDevice->SetIndices(ib);
		pd3dDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 
			0, 
			0, 
			mesh->GetNumVertices(), 
			meshContainer->pAttributeTable[materialID].FaceStart * 3, 
			meshContainer->pAttributeTable[materialID].FaceCount);
			
		//後始末
		pd3dDevice->SetStreamSourceFreq(0, 1);
		pd3dDevice->SetStreamSourceFreq(1, 1);

	}
	void CSkinModel::DrawMeshContainer(
		IDirect3DDevice9* pd3dDevice, 
		LPD3DXMESHCONTAINER pMeshContainerBase, 
		LPD3DXFRAME pFrameBase,
		ID3DXEffect* pEffect,
		D3DXMATRIX* worldMatrix,
		D3DXMATRIX* rotationMatrix,
		D3DXMATRIX* viewMatrix,
		D3DXMATRIX* projMatrix,
		CLight* light,
		bool isInstancingDraw,
		bool isDrawToShadowMap
	)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
		UINT iAttrib;
		LPD3DXBONECOMBINATION pBoneComb;

		D3DXMATRIXA16 matTemp;
		D3DXMATRIX viewProj;
		D3DXMatrixMultiply(&viewProj, viewMatrix, projMatrix);
			
		//テクニックを設定。
		{
			if (isInstancingDraw) {
				if (pMeshContainer->pSkinInfo != NULL) {
					if (isDrawToShadowMap) {
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_SkinModelInstancingRenderToShadowMap]);
					}
					else {
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_SkinModelInstancing]);
					}
				}
				else {
					if (isDrawToShadowMap) {
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_NoSkinModelInstancingRenderToShadowMap]);
	
					}
					else {
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_NoSkinModelInstancing]);

					}
				}
			}
			else {
				if (pMeshContainer->pSkinInfo != NULL) {
					if (isDrawToShadowMap) {
						//シャドウマップに描画。
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_SkinModelRenderShadowMap]);
					}else{
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_SkinModel]);
					}
				}
				else {
					if (isDrawToShadowMap) {
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_NoSkinModelRenderShadowMap]);
					}else{
						pEffect->SetTechnique(m_hShaderHandle[enShaderHandleTec_NoSkinModel]);
					}
				}
			}
		}
		//共通の定数レジスタを設定
		
		{
			//ビュープロジェクション
			pEffect->SetMatrix(m_hShaderHandle[enShaderHandleViewProj], &viewProj);
			pEffect->SetMatrix(m_hShaderHandle[enShaderHandleLastFrameViewProj], (D3DXMATRIX*)&MotionBlur().GetLastFrameViewProjectionMatrix());
			//ライト
			pEffect->SetValue(
				m_hShaderHandle[enShaderHandleLight],
				light,
				sizeof(CLight)
			);
			int flag[4] = { 0 };
			if (m_hasNormalMap) {
				//法線マップ。
				flag[0] = true;
			}
			if (!isDrawToShadowMap && m_isShadowReceiver) {
				//シャドウレシーバー。
				flag[1] = true;
				pEffect->SetTexture(m_hShaderHandle[enShaderHandleShadowMap_0], ShadowMap().GetTexture(0)->GetTextureDX());
				pEffect->SetTexture(m_hShaderHandle[enShaderHandleShadowMap_1], ShadowMap().GetTexture(1)->GetTextureDX());
				pEffect->SetTexture(m_hShaderHandle[enShaderHandleShadowMap_2], ShadowMap().GetTexture(2)->GetTextureDX());
				const CMatrix& mLVP = ShadowMap().GetLVPMatrix();
				pEffect->SetValue(m_hShaderHandle[enShaderHandleLVP], &mLVP, sizeof(mLVP));
				const CShadowMap::ShadowRecieverParam& srp = ShadowMap().GetShadowRecieverParam();
				pEffect->SetValue(m_hShaderHandle[enShaderHandleShadowRecieverParam], &srp, sizeof(srp));
			}
			if (m_isFresnel) {
				flag[2] = true;
			}
			CVector3 cameraPos;
			CVector3 cameraDir;
			D3DXMATRIX viewMatInv;
			D3DXMatrixInverse(&viewMatInv, NULL, viewMatrix);
			
			cameraPos.x = viewMatInv.m[3][0];
			cameraPos.y = viewMatInv.m[3][1];
			cameraPos.z = viewMatInv.m[3][2];
			cameraDir.x = viewMatInv.m[2][0];
			cameraDir.y = viewMatInv.m[2][1];
			cameraDir.z = viewMatInv.m[2][2];
			pEffect->SetVector(m_hShaderHandle[enShaderHandleCameraPos], (D3DXVECTOR4*)&cameraPos);
			pEffect->SetVector(m_hShaderHandle[enShaderHandleCameraDir], (D3DXVECTOR4*)&cameraDir);
			if (m_hasSpecMap) {
				//スペキュラマップ。
				flag[3] = true;
			}


			pEffect->SetValue(m_hShaderHandle[enShaderHandleFlags], flag, sizeof(flag));
			int flag2[4] = { 0 };
			if (m_isWriteVelocityMap) {
				flag2[0] = 1;
			}
			pEffect->SetValue(m_hShaderHandle[enShaderHandleFlags2], flag2, sizeof(flag2));
			if (isDrawToShadowMap || m_isShadowReceiver) {
				float farNear[] = {
					ShadowMap().GetFar(),
					ShadowMap().GetNear()
				};
				pEffect->SetValue(m_hShaderHandle[enShaderHandleFarNear], farNear, sizeof(farNear));
			}
			CVector4 fogParam;
			if (m_fogFunc == enFogFuncDist) {
				//距離フォグ
				fogParam.x = m_fogParam[0];
				fogParam.y = m_fogParam[1];
				fogParam.z = 1.0f;
			}
			else if (m_fogFunc == enFogFuncHeight) {
				//高さフォグ
				fogParam.x = m_fogParam[0];
				fogParam.y = m_fogParam[1];
				fogParam.z = 2.0f;
			}
			else {
				fogParam.z = 0.0f;
			}
			pEffect->SetVector(m_hShaderHandle[enShaderHandleFogParam], (D3DXVECTOR4*)&fogParam);
		}
		
		if (pMeshContainer->pSkinInfo != NULL)
		{
			//スキン情報有り。
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				// first calculate all the world matrices
				for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX)
					{
						TK_ASSERT(iPaletteEntry < MAX_MATRIX_PALLET, "ボーン行列パレットの最大数を超えた");
						TK_ASSERT(pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] != NULL, "NULL");
						D3DXMatrixMultiply(
							&m_boneMatrixPallet[iPaletteEntry],
							&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
						);
					}
				}
				
					
				pEffect->SetMatrixArray(m_hShaderHandle[enShaderHandleWorldMatrixArray], m_boneMatrixPallet, pMeshContainer->NumPaletteEntries);
				pEffect->SetInt(m_hShaderHandle[enShaderHandleNumBone], pMeshContainer->NumInfl);
				// マテリアルパラメータを転送。
				pMeshContainer->materials[pBoneComb[iAttrib].AttribId].SendMaterialParamToGPUImmidiate(pEffect);
				
				// ボーン数。
				pEffect->SetInt(m_hShaderHandle[enShaderHandleCurNumBones], pMeshContainer->NumInfl - 1);
				D3DXMATRIX viewRotInv;
				D3DXMatrixInverse(&viewRotInv, NULL, viewMatrix);
				viewRotInv.m[3][0] = 0.0f;
				viewRotInv.m[3][1] = 0.0f;
				viewRotInv.m[3][2] = 0.0f;
				viewRotInv.m[3][3] = 1.0f;
				D3DXMatrixTranspose(&viewRotInv, &viewRotInv);
				pEffect->SetMatrix(m_hShaderHandle[enShaderHandleViewMatrixRotInv], &viewRotInv);
				pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
				pEffect->BeginPass(0);
				pEffect->CommitChanges();
				// draw the subset with the current world matrix palette and material state
				if (isInstancingDraw) {
					//インスタンシング描画。
					DrawMeshContainer_InstancingDrawCommon(pd3dDevice, pMeshContainer, iAttrib);
				}
				else {
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
				}
				pEffect->EndPass();
				pEffect->End();

			}
		}
		else {
							
			D3DXMATRIX mWorld;
			if (pFrame != NULL) {
				mWorld = pFrame->CombinedTransformationMatrix;
			}
			else {
				mWorld = *worldMatrix;
			}
				
			pEffect->SetMatrix(m_hShaderHandle[enShaderHandleWorldMatrix], &mWorld);
			pEffect->SetMatrix(m_hShaderHandle[enShaderHandleRotationMatrix], rotationMatrix);
			pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
			pEffect->BeginPass(0);

			for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++) {
				pMeshContainer->materials[i].SendMaterialParamToGPUImmidiate(pEffect);
				pEffect->CommitChanges();
				if (isInstancingDraw) {
					//インスタンシング描画。
					DrawMeshContainer_InstancingDrawCommon(pd3dDevice, pMeshContainer, 0);
				}
				else {
					pMeshContainer->MeshData.pMesh->DrawSubset(i);
				}
			}
			pEffect->EndPass();
			pEffect->End();
		}
	}
	void CSkinModel::DrawFrame(
		IDirect3DDevice9* pd3dDevice,
		LPD3DXFRAME pFrame,
		ID3DXEffect* pEffect,
		D3DXMATRIX* worldMatrix,
		D3DXMATRIX* rotationMatrix,
		D3DXMATRIX* viewMatrix,
		D3DXMATRIX* projMatrix,
		bool isInstancingDraw,
		bool isDrawToShadowMap
	)
	{
		LPD3DXMESHCONTAINER pMeshContainer;

		pMeshContainer = pFrame->pMeshContainer;
		while (pMeshContainer != NULL)
		{
			DrawMeshContainer(
				pd3dDevice, 
				pMeshContainer, 
				pFrame, 
				pEffect,
				worldMatrix,
				rotationMatrix,
				viewMatrix,
				projMatrix,
				m_light,
				isInstancingDraw,
				isDrawToShadowMap
				);

			pMeshContainer = pMeshContainer->pNextMeshContainer;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			DrawFrame(
				pd3dDevice, 
				pFrame->pFrameSibling, 
				pEffect,
				worldMatrix,
				rotationMatrix,
				viewMatrix,
				projMatrix,
				isInstancingDraw,
				isDrawToShadowMap
				);
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			DrawFrame(
				pd3dDevice, 
				pFrame->pFrameFirstChild, 
				pEffect,
				worldMatrix,
				rotationMatrix,
				viewMatrix,
				projMatrix,
				isInstancingDraw,
				isDrawToShadowMap
				);
		}
	}
	
	CSkinModel::CSkinModel() :
		m_skinModelData(nullptr),
		m_worldMatrix(CMatrix::Identity),
		m_light(nullptr),
		m_isShadowCaster(false),
		m_isShadowReceiver(false),
		m_isFresnel(false),
		m_isReflectionCaster(false),
		m_fogFunc(enFogFuncNone),
		m_hasNormalMap(false),
		m_hasSpecMap(false)
	{
		m_fogParam[0] = 0.0f;
		m_fogParam[1] = 0.0f;
	}
	CSkinModel::~CSkinModel()
	{

	}

	void CSkinModel::Init(CSkinModelData* modelData)
	{
		m_pEffect = EffectManager().LoadEffect("Assets/presetShader/skinModel.fx");
		InitShaderConstHandle();
		m_skinModelData = modelData;
		m_shadowCaster.Create(this);
	}
	void CSkinModel::Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale)
	{
		if (m_isShadowCaster && ShadowMap().IsEnable()) {
			//シャドウキャスター。
			ShadowMap().Entry(&m_shadowCaster);
		}
		if (m_isReflectionCaster && ReflectionMap().IsEnable()) {
			//リフレクションキャスター。
			ReflectionMap().Entry(this);
		}
		CMatrix mTrans, mScale;
		mTrans.MakeTranslation(trans);
		m_rotationMatrix.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_worldMatrix.Mul(mScale, m_rotationMatrix);
		m_worldMatrix.Mul(m_worldMatrix, mTrans);
		if (m_skinModelData) {
			m_skinModelData->UpdateBoneMatrix(m_worldMatrix);	//ボーン行列を更新。
		}
	}
	void CSkinModel::InitShaderConstHandle()
	{
		ID3DXEffect* effectDx = m_pEffect->GetD3DXEffect();
		m_hShaderHandle[enShaderHandleLastFrameViewProj] = effectDx->GetParameterByName(NULL, "g_mViewProjLastFrame");
		m_hShaderHandle[enShaderHandleViewProj] 	= effectDx->GetParameterByName(NULL, "g_mViewProj");
		m_hShaderHandle[enShaderHandleLight] 		= effectDx->GetParameterByName(NULL, "g_light");
		m_hShaderHandle[enShaderHandleLVP] 			= effectDx->GetParameterByName(NULL, "g_mLVP");
		m_hShaderHandle[enShaderHandleCameraPos] 	= effectDx->GetParameterByName(NULL, "g_cameraPos");
		m_hShaderHandle[enShaderHandleFlags] 		= effectDx->GetParameterByName(NULL, "g_flags");
		m_hShaderHandle[enShaderHandleFlags2]		= effectDx->GetParameterByName(NULL, "g_flags2");
		m_hShaderHandle[enShaderHandleFarNear] 		= effectDx->GetParameterByName(NULL, "g_farNear");
		m_hShaderHandle[enShaderHandleFogParam] 	= effectDx->GetParameterByName(NULL, "g_fogParam");
		m_hShaderHandle[enShaderHandleWorldMatrixArray] 	= effectDx->GetParameterByName(NULL, "g_mWorldMatrixArray");
		m_hShaderHandle[enShaderHandleNumBone] 		= effectDx->GetParameterByName(NULL, "g_numBone");
		m_hShaderHandle[enShaderHandleCurNumBones] 	= effectDx->GetParameterByName(NULL, "CurNumBones");
		m_hShaderHandle[enShaderHandleViewMatrixRotInv] = effectDx->GetParameterByName(NULL, "g_viewMatrixRotInv");
		m_hShaderHandle[enShaderHandleWorldMatrix] = effectDx->GetParameterByName(NULL, "g_worldMatrix");
		m_hShaderHandle[enShaderHandleRotationMatrix] = effectDx->GetParameterByName(NULL, "g_rotationMatrix");
		m_hShaderHandle[enShaderHandleShadowMap_0] = effectDx->GetParameterByName(NULL, "g_shadowMap_0");
		m_hShaderHandle[enShaderHandleShadowMap_1] = effectDx->GetParameterByName(NULL, "g_shadowMap_1");
		m_hShaderHandle[enShaderHandleShadowMap_2] = effectDx->GetParameterByName(NULL, "g_shadowMap_2");
		m_hShaderHandle[enShaderHandleNormalTexture] = effectDx->GetParameterByName(NULL, "g_normalTexture");
		m_hShaderHandle[enShaderHandleSpeculerMap] = effectDx->GetParameterByName(NULL, "g_speculerMap");
		m_hShaderHandle[enShaderHandleDiffuseTexture] = effectDx->GetParameterByName(NULL, "g_diffuseTexture");
		m_hShaderHandle[enShaderHandleCameraDir] = effectDx->GetParameterByName(NULL, "g_cameraDir");
		m_hShaderHandle[enShaderHandleTec_SkinModelInstancingRenderToShadowMap] = effectDx->GetTechniqueByName("SkinModelInstancingRenderToShadowMap");
		m_hShaderHandle[enShaderHandleTec_SkinModelInstancing] = effectDx->GetTechniqueByName("SkinModelInstancing");
		m_hShaderHandle[enShaderHandleTec_NoSkinModelInstancingRenderToShadowMap] = effectDx->GetTechniqueByName("NoSkinModelInstancingRenderToShadowMap");
		m_hShaderHandle[enShaderHandleTec_NoSkinModelInstancing] = effectDx->GetTechniqueByName("NoSkinModelInstancing");
		m_hShaderHandle[enShaderHandleTec_SkinModelRenderShadowMap] = effectDx->GetTechniqueByName("SkinModelRenderShadowMap");
		m_hShaderHandle[enShaderHandleTec_SkinModel] = effectDx->GetTechniqueByName("SkinModel");
		m_hShaderHandle[enShaderHandleTec_NoSkinModelRenderShadowMap] = effectDx->GetTechniqueByName("NoSkinModelRenderShadowMap");
		m_hShaderHandle[enShaderHandleTec_NoSkinModel] = effectDx->GetTechniqueByName("NoSkinModel");
		m_hShaderHandle[enShaderHandleShadowRecieverParam] = effectDx->GetParameterByName(NULL, "gShadowReceiverParam");
	}
	/*!
	*@brief	シャドウマップに描画
	*@details
	* CShadowMapから呼び出されます。ユーザーは使用しないように。
	*/
	void CSkinModel::DrawToShadowMap(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		if (m_skinModelData) {
			CPIXPerfTag tag(renderContext, L"CSkinModel::DrawToShadowMap");
			renderContext.DrawSkinModelToShadowMap(this, viewMatrix, projMatrix);
		}
	}
	void CSkinModel::Draw(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		if (m_skinModelData) {
			CPIXPerfTag tag(renderContext, L"CSkinModel::Draw");
			renderContext.DrawSkinModel(this, viewMatrix, projMatrix);
		}
	}
	void CSkinModel::ImmidiateDraw(LPDIRECT3DDEVICE9 pd3ddevice, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, bool isDrawToShadowMap)
	{
		if (m_skinModelData) {
			DrawFrame(
				pd3ddevice, 
				m_skinModelData->GetFrameRoot(), 
				m_pEffect->GetD3DXEffect(),
				r_cast<D3DXMATRIX*>(&m_worldMatrix),
				r_cast<D3DXMATRIX*>(&m_rotationMatrix),
				viewMatrix,
				projMatrix,
				m_skinModelData->GetNumInstance() != 0,
				isDrawToShadowMap
			);
		}
	}
}