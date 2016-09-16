/*!
 * @brief	プリミティブ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkPrimitive.h"

namespace tkEngine{
	CPrimitive::CPrimitive() :
		m_numVertex(0),
		m_numIndex(0),
		m_vertexStride(0),
		m_type(eTriangleList),
		m_d3dPrimitiveType(D3DPT_TRIANGLELIST)
	{
	}
	CPrimitive::~CPrimitive()
	{
		Release();
	}
	void CPrimitive::Create( 
		EType 					primitiveType,
		int 					numVertex,
		int 					vertexStride,
		const SVertexElement*	vertexLayout,
		void*					pSrcVertexBuffer,
		int 					numIndex,
		EIndexFormat			indexFormat,
		void*					pSrcIndexbuffer
	)
	{
		TK_ASSERT( primitiveType < eTypeNum, "primitiveType is invalid" );
		TK_ASSERT( numVertex != 0, "numVertex is zero" );
		TK_ASSERT( vertexStride != 0, "vertexStrid is zero" );
		TK_ASSERT( numIndex != 0, "numIndex is zero" );
		m_type = primitiveType;
		m_numVertex = numVertex;
		m_vertexStride = vertexStride;
		m_numIndex = numIndex;
		Release();
		m_vertexBuffer.Create( numVertex, vertexStride, vertexLayout, pSrcVertexBuffer );
		m_indexBuffer.Create( numIndex, indexFormat, pSrcIndexbuffer );
		if(primitiveType == eTriangleList){
			m_numPolygon = numIndex / 3;
			m_d3dPrimitiveType = D3DPT_TRIANGLELIST;
		}else if(primitiveType == eTriangleStrip){
			m_numPolygon = numIndex - 2;
			m_d3dPrimitiveType = D3DPT_TRIANGLESTRIP;
		}
	}
	void CPrimitive::Release()
	{
		m_vertexBuffer.Release();
		m_indexBuffer.Release();
	}

}