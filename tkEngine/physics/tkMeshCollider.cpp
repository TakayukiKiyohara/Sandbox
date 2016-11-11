#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkMeshCollider.h"

namespace tkEngine{

	CMeshCollider::CMeshCollider() : 
		stridingMeshInterface(NULL)
	{
	}


	CMeshCollider::~CMeshCollider()
	{
		for (auto& vb : vertexBufferArray) {
			delete vb;
		}
		for (auto& ib : indexBufferArray) {
			delete ib;
		}
		delete stridingMeshInterface;
		delete meshShape;
	}

	/*!
	 * @brief	CSkinModelからメッシュコライダーを生成。
	 *@param[in]	model		スキンモデル。
	 */
	void CMeshCollider::CreateFromSkinModel( CSkinModel* model, const CMatrix* offsetMatrix )
	{
		stridingMeshInterface = new btTriangleIndexVertexArray;
		//CSkinModelからコリジョンで使用する、頂点バッファとインデックスバッファを作成する。
		LPD3DXMESH mesh = model->GetOrgMeshFirst();
		if (mesh != NULL) {
			{
				//頂点ストライドを取得。
				DWORD stride = D3DXGetFVFVertexSize(mesh->GetFVF());
				//頂点バッファを取得。
				LPDIRECT3DVERTEXBUFFER9 vb;
				mesh->GetVertexBuffer(&vb);
				//頂点バッファの定義を取得する。
				D3DVERTEXBUFFER_DESC desc;
				vb->GetDesc(&desc);
				//頂点バッファをロックする。
				CVector3* pos;
				vb->Lock(0, 0, (void**)&pos, D3DLOCK_READONLY);
				VertexBuffer* vertexBuffer = new VertexBuffer;
				int numVertex = mesh->GetNumVertices();
				//当たりデータで使用する頂点バッファを作成。
				for (int v = 0; v < numVertex; v++) {
					CVector3 posTmp = *pos;
					if (offsetMatrix) {
						offsetMatrix->Mul(posTmp);
					}
					vertexBuffer->push_back(posTmp);
					char* p = (char*)pos;
					p += stride;
					pos = (CVector3*)p;
				}
				vb->Unlock();
				vb->Release();
				vertexBufferArray.push_back(vertexBuffer);
			}
			{
				//続いてインデックスバッファを作成。
				LPDIRECT3DINDEXBUFFER9 ib;
				mesh->GetIndexBuffer(&ib);
				D3DINDEXBUFFER_DESC desc;
				ib->GetDesc(&desc);
				int stride = 0;
				if (desc.Format == D3DFMT_INDEX16) {
					//インデックスが16bit
					stride = 2;
				}
				else if(desc.Format == D3DFMT_INDEX32){
					//インデックスが32bit
					stride = 4;
				}
				//インデックスバッファをロック。
				char* p;
				HRESULT hr = ib->Lock(0, 0, (void**)&p, D3DLOCK_READONLY);
				IndexBuffer* indexBuffer = new IndexBuffer;
				for (int i = 0; i < (int)desc.Size / stride; i++) {
					unsigned int index;
					if (desc.Format == D3DFMT_INDEX16) {
						unsigned short* pIndex = (unsigned short*)p;
						index = (unsigned int)*pIndex;
					}
					else {
						unsigned int* pIndex = (unsigned int*)p;
						index = *pIndex;
					}
					
					indexBuffer->push_back(index);
					p += stride;
				}
				ib->Unlock();
				ib->Release();
				indexBufferArray.push_back(indexBuffer);
			}
			//インデックスメッシュを作成。
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = indexBufferArray.back();
			VertexBuffer* vb = vertexBufferArray.back();
			indexedMesh.m_numTriangles = (int)ib->size() / 3;
			indexedMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
			indexedMesh.m_triangleIndexStride = 12;
			indexedMesh.m_numVertices = (int)vb->size();
			indexedMesh.m_vertexBase = (unsigned char*)(&vb->front());
			indexedMesh.m_vertexStride = sizeof(CVector3);
			stridingMeshInterface->addIndexedMesh(indexedMesh);
			
		}
		meshShape = new btBvhTriangleMeshShape(stridingMeshInterface, true);	
	}
}