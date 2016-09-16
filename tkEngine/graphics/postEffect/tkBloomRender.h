/*!
 * @brief	�u���[���B
 */
#ifndef _TKBLOOM_H_
#define _TKBLOOM_H_


namespace tkEngine{
	/*!
	 * @brief	Bloom
	 */
	class CBloomRender{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CBloomRender();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CBloomRender();
		/*!
		 * @brief	�`��B
		 */
		void Render( CRenderContext& renderContext, CPostEffect* postEffect );
		/*!
		 * @brief	�쐬�B
		 */
		void Create( const SGraphicsConfig& config );
		/*!
		* @brief	�J���B
		*/
		void Release();
	private:
		/*!
		* @brief	�K�E�X�t�B���^�̏d�݂��X�V�B
		*/
		void UpdateWeight(float dispersion);
	private:
		static const int NUM_WEIGHTS = 8;
		static const int NUM_DOWN_SAMPLING_RT = 4;
		CEffect*		m_pEffect;						//!<�G�t�F�N�g�B
		CRenderTarget	m_luminanceRenderTarget;		//!<�P�x�𒊏o���郌���_�����O�^�[�Q�b�g�B
		CRenderTarget	m_downSamplingRenderTarget[NUM_DOWN_SAMPLING_RT];	//!<�P�x���_�E���T���v�����O���邽�߂̃����_�����O�^�[�Q�b�g�B
		bool			m_isEnable;						//!<�L���H
		float				m_weights[NUM_WEIGHTS];
	};
}

#endif // _TKBLOOM_H_