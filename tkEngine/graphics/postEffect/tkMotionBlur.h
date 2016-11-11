/*!
 * @brief	���x�}�b�v���g�p�������[�V�����u���[�B
 */

#pragma once

#include "tkEngine/Graphics/tkCamera.h"

namespace tkEngine{
	/*!
	 * @brief	���x�}�b�v���g�p�������[�V�����u���[�B
	 */
	class CMotionBlur : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CMotionBlur();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CMotionBlur();
		/*!
		* @brief	�쐬�B
		*@param[in]	config		�O���t�B�b�N�R���t�B�O�B
		*/
		void Create(const SGraphicsConfig& config);
		/*!
		* @brief	�X�V�B
		*/
		void Update()
		{
			if (m_isEnable) {
				m_lastFrameViewProjMatrix = m_camera->GetViewProjectionMatrix();
			}
		}
		/*!
		* @brief	���[�V�����u���[�̗L���A������ݒ�B
		*/
		void SetEnable(bool flag)
		{
			if (flag && !m_isEnable) {
				//�u���[����������L���ɕύX�����ꍇ�̓r���[�v���W�F�N�V�����s����X�V����B
				m_lastFrameViewProjMatrix = m_camera->GetViewProjectionMatrix();
			}
			m_isEnable = flag;
		}
		/*!
		* @brief	���[�V�����u���[�Ŏg�p����J������ݒ肷��B
		* @details
		* �@�u���[�����������V�[����`�悷��J������ݒ肵�Ă��������B
		*/
		void SetCamera(const CCamera* camera)
		{
			m_camera = camera;
			m_lastFrameViewProjMatrix = m_camera->GetViewProjectionMatrix();
		}
		/*!
		* @brief	1�t���[���O�̃r���[�v���W�F�N�V�����s��B
		*/
		const CMatrix& GetLastFrameViewProjectionMatrix()
		{
			return m_lastFrameViewProjMatrix;
		}
		/*!
		 * @brief	���x�}�b�v���������ނ��߂̃����_�����O�^�[�Q�b�g�B
		 */
		CRenderTarget* GetVelocityMapRenderTarget()
		{
			return &m_velocityMapRT;
		}
		/*!
		* @brief	�`��B
		*/
		void Render(CRenderContext& renderContext, CPostEffect* postEffect);
	private:
		const CCamera*		m_camera = nullptr;		//!<�J�����B
		CRenderTarget		m_velocityMapRT;		//!<���x�}�b�v���������ނ��߂̃����_�����O�^�[�Q�b�g�B
		bool				m_isEnable = false;		//!<���x�}�b�v�L���H
		CMatrix				m_lastFrameViewProjMatrix = CMatrix::Identity;	//!<1�t���[���O�̃r���[�v���W�F�N�V�����s��B
		CEffect*			m_effect = nullptr;		//!<�G�t�F�N�g�B
	};
}