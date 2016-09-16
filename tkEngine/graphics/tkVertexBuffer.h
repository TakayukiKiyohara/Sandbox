/*!
 * @brief	頂点バッファ
 */

#ifndef _TKRENDERBUFFER_H_
#define _TKRENDERBUFFER_H_

namespace tkEngine{
	enum EVertexFormat {
		eVertexFormat_xyz		= D3DFVF_XYZ,
		eVertexFormat_wyzrhw	= D3DFVF_XYZRHW,
		eVertexFormat_xyzw		= D3DFVF_XYZW,
		eVertexFormat_normal	= D3DFVF_NORMAL,
		eVertexFormat_diffuse	= D3DFVF_DIFFUSE,
		eVertexFormat_tex0		= D3DFVF_TEX0,
		eVertexFormat_tex1		= D3DFVF_TEX1,
		eVertexFormat_tex2		= D3DFVF_TEX2,
		eVertexFormat_tex3		= D3DFVF_TEX3,
		eVertexFormat_tex4		= D3DFVF_TEX4,
		eVertexFormat_tex5		= D3DFVF_TEX5,
		eVertexFormat_tex6		= D3DFVF_TEX6,
		eVertexFormat_tex7		= D3DFVF_TEX7,
		eVertexFormat_tex8		= D3DFVF_TEX8,
		eVertexFormat_invalid	= 0xffffffff,

	};
	/*!
	 * @brief	頂点バッファ
	 */
	class CVertexBuffer{
	public:
		CVertexBuffer();
		~CVertexBuffer();
		/*!
		 *@brief	頂点バッファの作成。
		 *@param[in]	numVertex		頂点数。
		 *@param[in]	stride			頂点ストライド。
		 *@param[in]	vertexLayout	頂点レイアウト。
		 *@param[in]	srcVertexBuffer	ソース頂点バッファ。作成された頂点バッファにコピーされます。NULLを指定可能。
		 */
		void Create( 
			int numVertex, 
			int stride, 
			const SVertexElement* vertexLayout, 
			const void* pSrcVertexBuffer 
		);
		/*!
		 * @brief	頂点バッファの開放。
		 */
		void Release();
		/*!
		* @brief	LPDIRECT3DVERTEXBUFFER9を取得。
		*/
		LPDIRECT3DVERTEXBUFFER9 GetBody()
		{
			return m_pVB;
		}
		/*!
		* @brief	頂点ストライドを取得。
		*/
		int GetStride() const
		{
			return m_stride;
		}
		/*!
		 * @brief	頂点定義を取得。
		 */
		SVertexDecralation* GetVertexDecl() 
		{
			return m_pVertexDecl;
		}
		/*!
		* @brief	頂点バッファを更新。
		*/
		void Update(const void* data);
	private:
		LPDIRECT3DVERTEXBUFFER9			m_pVB;				//!<頂点バッファ。
		SVertexDecralation* 			m_pVertexDecl;		//!<頂点定義。
		int 							m_stride;			//!<頂点ストライド。
		int 							m_numVertex;		//!<頂点数。
		int 							m_size;				//!<バッファサイズ。
	};
}
#endif // #define _TKRENDERBUFFER_H_