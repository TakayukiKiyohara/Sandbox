/*!
 *@brief	球
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/shape/tkSphereShape.h"
#include <vector>
namespace tkEngine {
	
	CSphereShape::CSphereShape()
	{
	}
	CSphereShape::~CSphereShape()
	{
	}
	void CSphereShape::Create(float radius, int grid, int color, bool hasNormal)
	{
		Release();
		m_isCreatePrimitive = true;
		//インデックスバッファの作成。
		std::vector<int>	indexBuffer;
		CreateIndexBuffer(indexBuffer, grid);
		if (hasNormal) {
			std::vector<SShapeVertex_PNC> vertexBuffer;
			CreateVertexPosition(vertexBuffer, radius, grid, color);
			//頂点マージ
			MergeVertex(vertexBuffer, indexBuffer, 0.001f );
			//法線を計算。
			CreateVertexNormal(vertexBuffer, indexBuffer, CPrimitive::eTriangleList);
			
			//プリミティブの作成。
			m_pPrimitive = new CPrimitive;
			m_pPrimitive->Create(
				CPrimitive::eTriangleList,
				vertexBuffer.size(),
				sizeof(SShapeVertex_PNC),
				scShapeVertex_PNC_Element,
				&vertexBuffer.at(0),
				(int)indexBuffer.size(),
				eIndexFormat32,
				&indexBuffer.at(0)
				);
		}
		else {
			//ポジション+座標頂点バッファの作成。
			std::vector<SShapeVertex_PC> vertexBuffer;
			CreateVertexPosition(vertexBuffer, radius, grid, color);
			//頂点マージ
			MergeVertex(vertexBuffer, indexBuffer, 0.01f );
			
			//プリミティブの作成。
			m_pPrimitive = new CPrimitive;
			m_pPrimitive->Create(
				CPrimitive::eTriangleList,
				vertexBuffer.size(),
				sizeof(SShapeVertex_PC),
				scShapeVertex_PC_Element,
				&vertexBuffer.at(0),
				(int)indexBuffer.size(),
				eIndexFormat32,
				&indexBuffer.at(0)
			);
		}
		CreateEffect(hasNormal);
	}
	void CSphereShape::CreateIndexBuffer(std::vector<int>& indexBuffer, int grid)
	{
		for (int i = 0; i < grid; i++) {
			int baseVertNo = (grid + 1) * i;
			int vertNo[3] = {
				baseVertNo ,
				baseVertNo + 1,
				baseVertNo + grid + 2
			};
			int vertNo2[3] = {
				baseVertNo + grid + 3,
				baseVertNo + grid + 2,
				baseVertNo + 2,
			};
			for (int k = 0; k < grid; k++) {
				if (k == 0) {
					indexBuffer.push_back(0);
					indexBuffer.push_back(vertNo[1]);
					indexBuffer.push_back(vertNo[2]);
					vertNo[0]++;
					vertNo[1]++;
				}
				else if (k == grid - 1) {
					indexBuffer.push_back(vertNo[0]);
					indexBuffer.push_back(vertNo[1]);
					indexBuffer.push_back(vertNo[2]);
				}
				else if (k != 0) {
					indexBuffer.push_back(vertNo[0]++);
					indexBuffer.push_back(vertNo[1]++);
					indexBuffer.push_back(vertNo[2]++);

					indexBuffer.push_back(vertNo2[0]++);
					indexBuffer.push_back(vertNo2[1]++);
					indexBuffer.push_back(vertNo2[2]++);
				}

			}
		}
	}
	template<class TVertex>
	void CSphereShape::CreateVertexPosition(std::vector<TVertex>& vertexBuffer, float radius, int grid, int color)
	{
		int numVertex = (grid + 1)*(grid + 1);
		vertexBuffer.reserve(numVertex);
		//球体形状の作成。
		float deltaAngle = 2.0f * CMath::PI / grid;
		float deltaAngle2 = CMath::PI / grid;
		for (int i = 0; i < grid + 1; i++)
		{
			float z_angle = deltaAngle * i;
			for (int k = 0; k < grid + 1; k++)
			{
				CVector3 t(0.0f, 0.0f, 0.0f);
				//X軸周りの回転。
				float x_angle = deltaAngle2 * k;
				t.y = sin(x_angle);
				t.z = cos(x_angle);
				//Z軸周りの回転。
				t.x = t.y * -sin(z_angle);
				t.y = t.y * cos(z_angle);
				t.Scale(radius);
				TVertex vtx;
				vtx.pos[0] = t.x;
				vtx.pos[1] = t.y;
				vtx.pos[2] = t.z;
				vtx.pos[3] = 1.0f;
				vtx.color = color;
				vertexBuffer.push_back(vtx);
			}
		}
	}
}