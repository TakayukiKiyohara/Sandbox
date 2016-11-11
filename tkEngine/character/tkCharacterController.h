/*!
 * @brief	キャラクタコントローラー。
 */

#pragma once

#include "tkEngine/Physics/tkSphereCollider.h"
#include "tkEngine/Physics/tkCapsuleCollider.h"
#include "tkEngine/Physics/tkRigidBody.h"

namespace tkEngine{
	/*!
	 * @brief	キャラクタコントローラー。
	 */
	class CCharacterController{
	public:
		CCharacterController() {

		}
		~CCharacterController()
		{

		}
		/*!
		 * @brief	初期化。
		 */
		void Init(float radius, float height, const CVector3& position);
		/*!
		 * @brief	実行。
		 */
		void Execute();
		/*!
		 * @brief	座標を取得。
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	座標を設定。
		 */
		void SetPosition(const CVector3& pos)
		{
			m_position = pos;
		}
		/*!
		 * @brief	移動速度を設定。
		 */
		void SetMoveSpeed(const CVector3& speed )
		{
			m_moveSpeed = speed;
		}
		/*!
		 * @brief	移動速度を取得。
		 */
		const CVector3& GetMoveSpeed() const
		{
			return m_moveSpeed;
		}
		/*!
		 * @brief	ジャンプさせる。
		 */
		void Jump( )
		{
			m_isJump = true;
			m_isOnGround = false;
		}
		/*!
		 * @brief	ジャンプ中か判定
		 */
		bool IsJump() const
		{
			return m_isJump;
		}
		/*!
		* @brief	地面上にいるか判定。
		*/
		bool IsOnGround() const
		{
			return m_isOnGround;
		}
		/*!
		* @brief	コライダーを取得。
		*/
		CCapsuleCollider* GetCollider()
		{
			return &m_collider;
		}
		/*!
		* @brief	重力を取得。
		*/
		void SetGravity(float gravity)
		{
			m_gravity = gravity;
		}
		/*!
		* @brief	剛体を取得。
		*/
		CRigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}
		/*!
		* @brief	剛体を物理エンジンから削除。。
		*/
		void RemoveRigidBoby();
	private:
		CVector3 			m_position = CVector3::Zero;	//座標。
		CVector3 			m_moveSpeed = CVector3::Zero;	//移動速度。 
		bool 				m_isJump = false;				//ジャンプ中？
		bool				m_isOnGround = true;			//地面の上にいる？
		CCapsuleCollider	m_collider;						//コライダー。
		float				m_radius = 0.0f;
		float				m_height = 0.0f;		
		CRigidBody			m_rigidBody;					//剛体。
		float				m_gravity = -9.8f;				//重力。
	};
}