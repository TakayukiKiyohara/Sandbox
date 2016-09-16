/*!
 * @brief	��ʊE�[�x
 */

#pragma once

#include "tkEngine/graphics/tkGaussianBlur.h"

namespace tkEngine{
	class CPostEffect;
	/*!
	 * @brief	��ʊE�[�x
	 */
	class CDof : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CDof();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CDof();
		/*!
		 * @brief	�`��B
		 */
		void Render( CRenderContext& renderContext, CPostEffect* postEffect );
		/*!
		* @brief	�쐬�B
		*@param[in]	config		�O���t�B�b�N�R���t�B�O�B
		*/
		void Create(const SGraphicsConfig& config);
		/*!
		* @brief	�[�x���������ރ����_�����O�^�[�Q�b�g�̎擾�B
		*/
		CRenderTarget* GetDepthRenderTarget()
		{
			return &m_depthRT;
		}
		/*!
		* @brief	�J���B
		*/
		void Release();
		/*!
		* @brief	�s���g��ݒ�B(�J�������W�n)�B
		*@param[in]	pintPosition	�s���g(�P��mm)�B
		*/
		void SetPint(float pint)
		{
			m_pint = pint;
		}
		/*!
		* @brief	�œ_������ݒ�B
		*@param[in]		length		�œ_����(�P�ʂ�mm)
		*/
		void SetFocalLength(float length)
		{
			m_focalLength = length;
		}
		/*!
		* @brief	F�l��ݒ�B
		*@param[in]		f	F�l
		*/
		void SetFParam(float f)
		{
			m_F = f;
		}
	private:
		CRenderTarget		m_depthRT;		//!<�[�x���������ރ����_�����O�^�[�Q�b�g�B
		bool				m_isEnable;		//!<DOF�L���H
		CGaussianBlur		m_blurForward;	//!<�O�{�P�p�̃u���[�B
		CGaussianBlur		m_blurBack;		//!<���{�P�p�̃u���[�B
		CEffect*			m_effect;		//!<�G�t�F�N�g�B
		float				m_focalLength;	//!<�œ_�����B
		float				m_F;			//!<F�l�B
		float				m_pint;
	};
}
