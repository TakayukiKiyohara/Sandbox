/*!
 * @brief	�L�����N�^�R���g���[���[�B
 */

#pragma once

#include "tkEngine/Physics/tkSphereCollider.h"
#include "tkEngine/Physics/tkCapsuleCollider.h"
#include "tkEngine/Physics/tkRigidBody.h"

namespace tkEngine{
	/*!
	 * @brief	�L�����N�^�R���g���[���[�B
	 */
	class CCharacterController{
	public:
		CCharacterController() {

		}
		~CCharacterController()
		{

		}
		/*!
		 * @brief	�������B
		 */
		void Init(float radius, float height, const CVector3& position);
		/*!
		 * @brief	���s�B
		 */
		void Execute();
		/*!
		 * @brief	���W���擾�B
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	���W��ݒ�B
		 */
		void SetPosition(const CVector3& pos)
		{
			m_position = pos;
		}
		/*!
		 * @brief	�ړ����x��ݒ�B
		 */
		void SetMoveSpeed(const CVector3& speed )
		{
			m_moveSpeed = speed;
		}
		/*!
		 * @brief	�ړ����x���擾�B
		 */
		const CVector3& GetMoveSpeed() const
		{
			return m_moveSpeed;
		}
		/*!
		 * @brief	�W�����v������B
		 */
		void Jump( )
		{
			m_isJump = true;
			m_isOnGround = false;
		}
		/*!
		 * @brief	�W�����v��������
		 */
		bool IsJump() const
		{
			return m_isJump;
		}
		/*!
		* @brief	�n�ʏ�ɂ��邩����B
		*/
		bool IsOnGround() const
		{
			return m_isOnGround;
		}
		/*!
		* @brief	�R���C�_�[���擾�B
		*/
		CCapsuleCollider* GetCollider()
		{
			return &m_collider;
		}
		/*!
		* @brief	�d�͂��擾�B
		*/
		void SetGravity(float gravity)
		{
			m_gravity = gravity;
		}
		/*!
		* @brief	���̂��擾�B
		*/
		CRigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}
		/*!
		* @brief	���̂𕨗��G���W������폜�B�B
		*/
		void RemoveRigidBoby();
	private:
		CVector3 			m_position = CVector3::Zero;	//���W�B
		CVector3 			m_moveSpeed = CVector3::Zero;	//�ړ����x�B 
		bool 				m_isJump = false;				//�W�����v���H
		bool				m_isOnGround = true;			//�n�ʂ̏�ɂ���H
		CCapsuleCollider	m_collider;						//�R���C�_�[�B
		float				m_radius = 0.0f;
		float				m_height = 0.0f;		
		CRigidBody			m_rigidBody;					//���́B
		float				m_gravity = -9.8f;				//�d�́B
	};
}