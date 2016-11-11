/*!
 * @brief	�e�N�X�`���B
 */

#ifndef _TKTEXTURE_H_
#define _TKTEXTURE_H_

namespace tkEngine{
	/*!
	 * @brief	�e�N�X�`���B
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
		//�e�N�X�`���̕����擾�B
		int GetWidth() const
		{
			return m_texW;
		}
		//�e�N�X�`���̍������擾�B
		int GetHeight() const
		{
			return m_texH;
		}
		/*!
		 * @brief	�e�N�X�`�������[�h�B
		 */
		bool Load( const char* fileName );
	private:
		//�e�N�X�`���T�C�Y���v�Z�B
		void ComputeTexSize()
		{
			D3DSURFACE_DESC desc;
			m_tex->GetLevelDesc(0, &desc);
			m_texW = desc.Width;
			m_texH = desc.Height;
		}
	private:
		LPDIRECT3DTEXTURE9	m_tex = nullptr;	//!<�e�N�X�`���B
		int m_texW = 0;							//!<�e�N�X�`���̉����B
		int m_texH = 0;							//!<�e�N�X�`���̏c���B
	};
}

#endif // _TKTEXTURE_H_