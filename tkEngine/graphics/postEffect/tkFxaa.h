/*!
 *@brief	FXAA
 */

#pragma once

namespace tkEngine{
	class CPostEffect;
	/*!
	 *@brief	FXAA
	 *@details
	 * �|�X�g�G�t�F�N�g�ɂ��A���`�G�C���A�X
	 */
	class CFxaa {
	public:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CFxaa();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CFxaa();
		/*!
		* @brief	�J���B
		*/
		void Release();
		/*!
		 *@brief	�������B
		 *@param[in]	config		�O���t�B�b�N�R���t�B�O
		 */
		void Init(const SGraphicsConfig& config);
		/*!
		* @brief	�`��B
		*/
		void Render(CRenderContext& renderContext, CPostEffect* postEffect);
	private:
		bool			m_isEnable;	//�A���`���L�����ǂ����̃t���O�B
		CEffect*		m_effect;
	};
}