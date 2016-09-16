/*!
 *@brief	インデックスバッファ
 */

#ifndef _TKINDEXBUFFER_H_
#define _TKINDEXBUFFER_H_

namespace tkEngine{
	enum EIndexFormat {
		eIndexFormat16 = D3DFMT_INDEX16,	//16bitインデックス。
		eIndexFormat32 = D3DFMT_INDEX32,	//32bitインデックス。
	};
	class CIndexBuffer{
	public:
		CIndexBuffer();
		~CIndexBuffer();
		/*!
		 *@brief	インデックスバッファの作成。
		 *@param[in]	numIndex		インデックスの数
		 *@param[in]	format			インデックスバッファのフォーマット。
		 *@param[in]	pSrcIndexBuffer	ソースインデックスバッファ。作成されたインデックスバッファにコピーされます。NULLを指定可能。
		 */
		void Create( int numIndex, EIndexFormat format, const void* pSrcIndexBuffer );
		/*!
		 *@brief	インデックスバッファの解放。
		 */
		void Release();
		/*!
		*@brief	LPDIRECT3DINDEXBUFFER9の取得。
		*/
		LPDIRECT3DINDEXBUFFER9 GetBody()
		{
			return m_pIB;
		}
	private:
		LPDIRECT3DINDEXBUFFER9	m_pIB;	//!<インデックスバッファ。
	};
}

#endif //_TKINDEXBUFFER_H_