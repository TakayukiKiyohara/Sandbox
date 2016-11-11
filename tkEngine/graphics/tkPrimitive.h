/*!
 * @brief	プリミティブ。
 */

#ifndef _TKPRIMITIVE_H_
#define _TKPRIMITIVE_H_

#include "tkEngine/graphics/tkIndexBuffer.h"
#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine{
	/*!
	 * @brief	プリミティブ。
	 */
	class CPrimitive{
	public:
		/*!
		 * @brief	プリミティブタイプ。
		 */
		enum EType{
			eTriangleList,		//!<トライアングルリスト。
			eTriangleStrip,		//!<トライアングルストリップ。
			eTypeNum,
		};
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPrimitive();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPrimitive();
		/*!
		 *@brief	プリミティブの作成。
		 *@param[in]	primitiveType		プリミティブタイプ。
		 *@param[in]	numVertex			頂点数
		 *@param[in]	vertexStride		頂点ストライド
		 *@param[in]	vertexLayout		頂点レイアウト。EVertexFormatの論理和を渡す。
		 *@param[in]	pSrcVertexBuffer	ソース頂点バッファ。
		 *@param[in]	numIndex			インデックスの数
		 *@param[in]	indexFormat			インデックスバッファのフォーマット。
		 *@param[in]	pSrcIndexBuffer		ソースインデックスバッファ。
		 */
		void Create( 
			EType 					primitiveType,
			int 					numVertex,
			int 					vertexStride,
			const SVertexElement*	vertexLayout,
			void*					pSrcVertexBuffer,
			int 					numIndex,
			EIndexFormat			indexFormat,
			void*					pSrcIndexbuffer
		);
		/*!
		 * @brief	リリース。
		 */
		void Release();
		/*!
		* @brief	頂点バッファの取得。
		*/
		CVertexBuffer* GetVertexBuffer() 
		{
			return &m_vertexBuffer;
		}
		/*!
		* @brief	インデックスバッファの取得。
		*/
		CIndexBuffer*  GetIndexBuffer()
		{
			return &m_indexBuffer;
		}
		/*!
		* @brief	D3DPRIMITIVETYPEを取得。
		*/
		D3DPRIMITIVETYPE GetD3DPrimitiveType() const
		{
			return m_d3dPrimitiveType;
		}
		/*!
		* @brief	頂点数を取得。
		*/
		int GetNumVertex() const
		{
			return m_numVertex;
		}
		/*!
		* @brief	ポリゴン数を取得。
		*/
		int GetNumPolygon() const
		{
			return m_numPolygon;
		}
		/*!
		* @brief	頂点定義を取得。
		*/
		SVertexDecralation* GetVertexDecl()
		{
			return m_vertexBuffer.GetVertexDecl();
		}
	private:
		int					m_numVertex;		//!<頂点数。
		int					m_vertexStride;		//!<頂点ストライド。
		int					m_numIndex;			//!<インデックス数。
		int					m_numPolygon;		//!<ポリゴンの数。
		CVertexBuffer		m_vertexBuffer;		//!<頂点バッファ。
		CIndexBuffer		m_indexBuffer;		//!<インデックスバッファ。
		EType				m_type;				//!<プリミティブタイプ。
		D3DPRIMITIVETYPE	m_d3dPrimitiveType;	//!<D3DPRIMITIVETYPE。
	};
}
#endif // _TKPRIMITIVE_H_