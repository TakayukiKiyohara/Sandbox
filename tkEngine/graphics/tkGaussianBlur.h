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
		enum EnUseWeight {
			enUseWeight_2,		//!<�E�F�C�g2�{�B
			enUseWeight_4,		//!<�E�F�C�g4�{�B
			enUseWeight_6,		//!<�E�F�C�g6�{�B
			enUseWeight_8,		//!<�E�F�C�g8�{�B
			enNumUseWeight,
		};
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
		/*!
		* @brief	�g�p����E�F�C�g�̐���ݒ�B
		*@param[in]	w		EnUseWeight���Q�ƁB
		*/
		void SetUseWeights(EnUseWeight w)
		{
			m_useWeights = w;
		}
	private:
		void UpdateWeight(float dispersion);
	private:
		
		CRenderTarget		m_rt[2];			//!<�K�E�V�A���u���[�̌��ʂ�`�����ރ����_�����O�^�[�Q�b�g�B
		const CTexture*		m_srcTexture;		//!<�\�[�X�ƂȂ�e�N�X�`���B
		CPrimitive			m_prim;				//!<�v���~�e�B�u�B
		CEffect*			m_effect;			//!<�G�t�F�N�g�B
		int					m_srcTexWH[2]; 
		EnUseWeight			m_useWeights;		//!<�g�p����E�F�C�g�̐��B
		static const int	MAX_WEIGHTS = 8;
		float				m_weights[MAX_WEIGHTS];
		float				m_blurPower;
	};
}