/*!
 * @brief	�J����
 */

#ifndef _TKCAMERA_H_
#define _TKCAMERA_H_

namespace tkEngine{
	class CCamera{
	public:
		CCamera();
		~CCamera();
		/*!
		 * @brief	�r���[�s��A�v���W�F�N�V�����s��A�r���[�v���W�F�N�V�����s����X�V�B
		 */
		void Update();
		/*!
		 * @brief	�J�����̍��W��ݒ�B
		 *@param[in]	pos		�J�����̍��W�B
		 */
		void SetPosition( const CVector3& pos ) 
		{
			m_position = pos;
		}
		/*!
		 * @brief	�J�����̍��W���擾
		 *@return	�J�����̍��W�B
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	�����_��ݒ�B
		 *@param[in]	target		�����_�B
		 */
		void SetTarget( const CVector3& target )
		{
			m_target = target;
		}
		/*!
		 * @brief	�����_���擾�B
		 *@return	�����_�B
		 */
		const CVector3& GetTarget() const
		{
			return m_target;
		}
		/*!
		 * @brief	�J�����̏������ݒ�B
		 */
		void SetUp( const CVector3& up )
		{
			m_up = up;
		}
		/*!
		 * @brief	�J�����̏�������擾
		 */
		const CVector3& GetUp() const
		{
			return m_up;
		}
		/*!
		* @brief	�r���[�s��̋t�s����擾�B
		*/
		const CMatrix& GetViewMatrixInv() const
		{
			return m_viewMatrixInv;
		}
		/*!
		 * @brief	�r���[�s����擾�B
		 */
		const CMatrix& GetViewMatrix() const
		{
			return m_viewMatrix;
		}
		/*!
		 * @brief	�v���W�F�N�V�����s����擾�B
		 */
		const CMatrix& GetProjectionMatrix() const
		{
			return m_projectionMatrix;
		}
		/*!
		 * @brief	�r���[�v���W�F�N�V�����s����擾�B
		 */
		const CMatrix& GetViewProjectionMatrix() const
		{
			return m_viewProjectionMatrix;
		}
		/*!
		* @brief	�J�����̉�]�s����擾�B
		*/
		const CMatrix& GetCameraRotation() const
		{
			return m_cameraRotation;
		}
		/*!
		 * @brief	�����ʂ�ݒ�B
		 */
		void SetFar( float fFar )
		{
			m_far = fFar;
		}
		/*!
		 * @brief	�ߕ��ʂ�ݒ�B
		 */
		void SetNear( float fNear )
		{
			m_near = fNear;
		}
		/*!
		 * @brief	�����ʂ��擾�B
		 */
		float GetFar() const
		{
			return m_far;
		}
		/*!
		 * @brief	�ߕ��ʂ��擾�B
		 */
		float GetNear() const
		{
			return m_near;
		}
		/*!
		* @brief	��p��ݒ�B
		*/
		void SetViewAngle(float viewAngle)
		{
			m_viewAngle = viewAngle;
		}
		/*!
		* @brief	��p���擾�B
		*/
		float GetViewAngle() const
		{
			return m_viewAngle;
		}
	private:
		CVector3	m_position;							//!<�J�����ʒu�B
		CVector3	m_up;								//!<�J�����̏�����B
		CVector3	m_target;							//!<�J�����̒��~�_�B
		CMatrix		m_viewMatrix;						//!<�r���[�s��B
		CMatrix		m_projectionMatrix;					//!<�v���W�F�N�V�����s��B
		CMatrix		m_viewProjectionMatrix;				//!<�r���[�v���W�F�N�V�����s��B
		CMatrix		m_viewMatrixInv;					//!<�r���[�s��̋t�s��B
		CMatrix		m_cameraRotation;					//!<�J�����̉�]�s��B
		float			m_near;								//!<�ߕ��ʁB
		float			m_far;								//!<�����ʁB
		float			m_viewAngle;						//!<��p(���W�A��)�B
		float			m_aspect;							//!<�A�X�y�N�g��B
		bool		m_isNeedUpdateProjectionMatrix;
	};
}
#endif //_TKCAMERA_H_