/*!
 * @brief	�V���h�E�}�b�v�B
 */

#ifndef _TKSHADOWMAP_H_
#define _TKSHADOWMAP_H_

#include "tkEngine/graphics/tkBlur.h"


namespace tkEngine{
	class IShadowCaster;
	class CCamera;
	class CShadowMap : Noncopyable{
	public:
		//!<���C�g�r���[�̌v�Z�̎d���B
		enum EnCalcLightViewFunc {
			enCalcLightViewFunc_PositionDirection,	//���C�g�̈ʒu�ƕ����Ōv�Z����B
			enCalcLightViewFunc_PositionTarget,		//���C�g�̈ʒu�ƒ����_�Ōv�Z����B
		};
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CShadowMap();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CShadowMap();
		/*!
		 * @brief	�쐬�B
		 */
		void Create( int w, int h );
		/*!
		 * @brief	�J���B
		 */
		void Release();
		/*!
		 * @brief	�V���h�E�L���X�^�[���G���g���[�B
		 */
		void Entry( IShadowCaster* caster );
		/*!
		 * @brief	�V���h�E�}�b�v�ɏ������݁B
		 *@param[in]	renderContext		�����_�����O�R���e�L�X�g�B
		 */
		void RenderToShadowMap( CRenderContext& renderContext );
		/*!
		 * @brief	�V���h�E�}�b�v�̃e�N�X�`�����擾�B
		 */
		const CTexture* GetTexture() const
		{
#ifdef USE_VSM
			return m_blur.GetTexture();
#else
			return m_shadowMapRT.GetTexture();
#endif
		}
		/*!
		 * @brief	���C�g�̕�����ݒ�B
		 *@details
		 * ���C�g�r���[�̌v�Z���@��enCalcLightViewFunc_PositionDirection�̎��ɗL���ɂȂ�B
		 */
		void SetLightDirection( const CVector3& lightDir )
		{
			m_lightDirection = lightDir;
		}
		/*!
		* @brief	���C�g�r���[�̒����_��ݒ�B
		*@details
		*  ���C�g�r���[�̌v�Z���@��enCalcLightViewFunc_PositionTarget�̎��ɗL���ɂȂ�B
		*/
		void SetLightTarget(const CVector3& lightTarget)
		{
			m_lightTarget = lightTarget;
		}
		/*!
		 * @brief	���C�g�̈ʒu��ݒ�B
		 */
		void SetLightPosition( const CVector3& lightPosition )
		{
			m_lightPosition = lightPosition;
		}
		/*!
		* @brief	���C�g�r���[�v���W�F�N�V�����s����擾�B
		*/
		const CMatrix& GetLVPMatrix() const
		{
			return m_LVPMatrix;
		}
		/*!
		* @brief	�����ʂ�ݒ�B
		*/
		void SetFar(float Far)
		{
			m_far = Far;
		}
		/*!
		* @brief	�����ʂ�ݒ�B
		*/
		void SetNear(float Near)
		{
			m_far = Near;
		}
		/*!
		* @brief	�X�V�B
		*/
		void Update();
		/*!
		* @brief	�ߕ��ʁB
		*/
		float GetNear() const
		{
			return m_near;
		}
		/*!
		* @brief	������
		*/
		float GetFar() const
		{
			return m_far;
		}
		/*!
		* @brief	�V���h�E�}�b�v���L��������B
		*/
		bool IsEnable() const
		{
			return m_isEnable;
		}
		/*!
		* @brief	�J������ݒ�BPSM���s���Ƃ��Ɏg�p�����B
		*/
		void SetCamera(CCamera* camera)
		{
			m_camera = camera;
		}
		/*!
		* @brief	���C�g�r���[�̌v�Z�̎d����ݒ�B
		*/
		void SetCalcLightViewFunc(EnCalcLightViewFunc func)
		{
			m_calcLightViewFunc = func;
		}
	private:
		bool						m_isEnable;				//!<�L���H
		CRenderTarget				m_shadowMapRT;			//!<�V���h�E�}�b�v���������ރ����_�����O�^�[�Q�b�g�B
		std::vector<IShadowCaster*>	m_shadowCaster;			//!<�V���h�E�L���X�^�[�B
		CEffect*					m_pShadowMapEffect;		//!<�V���h�E�}�b�v�̃G�t�F�N�g�B
		CVector3					m_lightPosition;		//!<���C�g�̈ʒu�B
		CVector3					m_lightDirection;		//!<���C�g�̌����B
		CVector3					m_lightTarget;			//!<�����_
		CMatrix						m_lvMatrix;
		CMatrix						m_LVPMatrix;			//!<���C�g�r���[�v���W�F�N�V�����s��B
		CMatrix						m_projectionMatrix;
		float						m_near;					//!<�ߕ��ʁB
		float						m_far;					//!<�����ʁB
		float						m_accpect;
		float						m_shadowAreaW;			//!<�e�𗎂Ƃ��͈͂̕��B
		float						m_shadowAreaH;			//!<�e�𗎂Ƃ��͈͂̍����B
		CCamera*					m_camera;				//!<PSM���v�Z����Ƃ��Ɏg�p����J�����B
		EnCalcLightViewFunc			m_calcLightViewFunc;	//!<���C�g�r���[�̌v�Z���@�B
#ifdef USE_VSM
		CBlur						m_blur;			//!<�[�x�}�b�v�̕��ϒl�����߂邽�߂̃u���[�����B
#endif 
	};
}

#endif //_TKSHADOWMAP_H_