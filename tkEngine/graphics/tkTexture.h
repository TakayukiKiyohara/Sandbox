/*!
 * @brief	テクスチャ。
 */

#ifndef _TKTEXTURE_H_
#define _TKTEXTURE_H_

namespace tkEngine{
	/*!
	 * @brief	テクスチャ。
	 */
	class CTexture : Noncopyable{
	public:
		CTexture() :
			m_tex(nullptr)
		{
		}
		~CTexture()
		{
			Release();
		}
		void SetTextureDX( LPDIRECT3DTEXTURE9 tex )
		{
			m_tex = tex;
		}
		LPDIRECT3DTEXTURE9 GetTextureDX() const
		{
			return m_tex;
		}
		void Release()
		{
			if (m_tex) {
				m_tex->Release();
				m_tex = nullptr;
			}
		}
		/*!
		 * @brief	テクスチャをロード。
		 */
		void Load( const char* fileName );
	private:
		LPDIRECT3DTEXTURE9	m_tex;		//!<テクスチャ。
	};
}

#endif // _TKTEXTURE_H_