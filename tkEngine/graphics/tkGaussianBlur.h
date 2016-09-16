/*!
 * @brief	�K�E�V�A���u���[�B
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	�K�E�V�A���u���[�B
	 */
	class CGaussianBlur{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CGaussianBlur();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CGaussianBlur();
		void Init( int w, int h, const CTexture& srcTexture );
		/*!
		* @brief	�\�[�X�e�N�X�`����ݒ�B
		*@details
		* Init�Ŏg�p�����e�N�X�`���Ɠ����T�C�Y�̂��̂�n���悤�ɁB
		*@param[in]	texture		�\�[�X�e�N�X�`���B
		*/
		void SetSrcTexture(const CTexture& texture)
		{
			m_srcTexture = &texture;
		}
		void Draw(CRenderContext& renderContext);
		const CTexture* GetTexture() const
		{
			return m_rt[1].GetTexture();
		}
		/*!
		* @brief	�u���[�̋��x��ݒ�B
		*@details
		* Init�Ŏg�p�����e�N�X�`���Ɠ����T�C�Y�̂��̂�n���悤�ɁB
		*@param[in]	texture		�\�[�X�e�N�X�`���B
		*/
		void SetBlurPower(float blurPower)
		{
			m_blurPower = blurPower;
		}
	private:
		void UpdateWeight(float dispersion);
	private:
		CRenderTarget		m_rt[2];			//!<�K�E�V�A���u���[�̌��ʂ�`�����ރ����_�����O�^�[�Q�b�g�B
		const CTexture*		m_srcTexture;		//!<�\�[�X�ƂȂ�e�N�X�`���B
		CPrimitive			m_prim;				//!<�v���~�e�B�u�B
		CEffect*			m_effect;			//!<�G�t�F�N�g�B
		int					m_srcTexWH[2]; 
		static const int NUM_WEIGHTS = 8;
		float				m_weights[NUM_WEIGHTS];
		float				m_blurPower;
	};
}