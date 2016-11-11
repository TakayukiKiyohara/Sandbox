/*!
 * @brief	テクスチャ。
 */

#ifndef _TKTEXTURE_H_
#define _TKTEXTURE_H_

namespace tkEngine{
	/*!
	 * @brief	テクスチャ。
	 */
	class CTexture{
	public:
		CTexture()
		{
		}
		~CTexture()
		{
			Release();
		}
		void SetTextureDX( LPDIRECT3DTEXTURE9 tex )
		{
			m_tex = tex;
			ComputeTexSize();
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
		//テクスチャの幅を取得。
		int GetWidth() const
		{
			return m_texW;
		}
		//テクスチャの高さを取得。
		int GetHeight() const
		{
			return m_texH;
		}
		/*!
		 * @brief	テクスチャをロード。
		 */
		bool Load( const char* fileName );
	private:
		//テクスチャサイズを計算。
		void ComputeTexSize()
		{
			D3DSURFACE_DESC desc;
			m_tex->GetLevelDesc(0, &desc);
			m_texW = desc.Width;
			m_texH = desc.Height;
		}
	private:
		LPDIRECT3DTEXTURE9	m_tex = nullptr;	//!<テクスチャ。
		int m_texW = 0;							//!<テクスチャの横幅。
		int m_texH = 0;							//!<テクスチャの縦幅。
	};
}

#endif // _TKTEXTURE_H_