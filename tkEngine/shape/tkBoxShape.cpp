/*!
 * @brief	ボックス
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/shape/tkBoxShape.h"

namespace tkEngine{
	CBoxShape::CBoxShape() 
	{
	}
	CBoxShape::~CBoxShape()
	{
		Release();
	}
	void CBoxShape::Create( CVector3 size, int color, bool hasNormal )
	{
		Release();
		m_isCreatePrimitive = true;
		CVector3 halfSize = size;
		halfSize.Scale(0.5f);
		SShapeVertex_PC vertex[] = {
			//前面
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color,
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color,
			},
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color,
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color,
			},
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color,
			},
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color,
			},
			//裏面。
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color,
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color,
			},
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color,
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color,
			},
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color,
			},
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color,
			},
			//左面。
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color 
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color 
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color 
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color 
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color 
			},
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color 
			},
			//右面
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color
			},
			//下面
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				color
			},
			//上面
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				color
			},

		};

		SShapeVertex_PNC vertexPNC[] = {
			//前面
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			//裏面。
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color,
			},
			//左面。
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			//右面
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			//下面
			{
				-halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, -halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			//上面
			{
				halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},
			{
				-halfSize.x, halfSize.y, -halfSize.z, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				color
			},

		};

		static unsigned short index[] = {
			0,  1, 2, 3, 4, 5, 
			6,  7, 8, 9,10,11,
			12,13,14,15,16,17,
			18,19,20,21,22,23,
			24,25,26,27,28,29,
			30,31,32,33,34,35,
		};
		m_pPrimitive = new CPrimitive;
		if (hasNormal) {
			//法線を作成。
			CreateVertexNormalArray(
				vertexPNC,
				sizeof(vertexPNC) / sizeof(vertexPNC[0]),
				index,
				sizeof(index)/sizeof(index[0]),
				CPrimitive::eTriangleList
			);
			m_pPrimitive->Create(
				CPrimitive::eTriangleList,
				sizeof(vertexPNC) / sizeof(vertexPNC[0]),
				sizeof(vertexPNC[0]),
				scShapeVertex_PNC_Element,
				vertexPNC,
				sizeof(index) / sizeof(index[0]),
				eIndexFormat16,
				index
				);
		}
		else {
			m_pPrimitive->Create(
				CPrimitive::eTriangleList,
				sizeof(vertex) / sizeof(vertex[0]),
				sizeof(vertex[0]),
				scShapeVertex_PC_Element,
				vertex,
				sizeof(index) / sizeof(index[0]),
				eIndexFormat16,
				index
				);
		}
		CreateEffect(hasNormal);
	}
}
