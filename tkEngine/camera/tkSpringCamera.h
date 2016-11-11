/*!
 * @brief	バネカメラ
 */

#pragma once

#include "tkEngine/graphics/tkCamera.h"

namespace tkEngine{
	/*!
	 * @brief	バネカメラ。
	 */
	class CSpringCamera{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CSpringCamera();
		/*!
		 * @brief	デストラクタ
		 */
		~CSpringCamera();
		/*!
		* @brief	初期化。
		*/
		void Init( const CVector3& target, const CVector3& position, float maxMoveSpeed);
		/*!
		 * @brief	注視点を設定。
		 */
		void SetTarget( const CVector3& target )
		{
			m_target = target;
		}
		/*!
		 * @brief	視点を設定。
		 */
		void SetPosition( const CVector3 position )
		{
			m_position = position ;
		}
		/*!
		* @brief	遠平面を設定。
		*/
		void SetFar(float _far)
		{
			m_camera.SetFar(_far);
		}
		/*!
		* @brief	カメラの回転行列を取得。
		*/
		const CMatrix& GetCameraRotation() const
		{
			return m_camera.GetCameraRotation();
		}
		/*!
		* @brief	注視点を取得。
		*/
		const CVector3& GetTarget() const
		{
			return m_camera.GetTarget();
		}
		/*!
		* @brief	視点を取得。
		*/
		const CVector3& GetPosition() const
		{
			return m_camera.GetPosition();
		}
		/*!
		 * @brief	更新。
		 */
		void Update();
		/*!
		 * @brief	カメラを取得。
		 */
		const CCamera* GetCamera() const
		{
			return &m_camera;
		}
		/*!
		* @brief	バネの減衰率を設定。
		*@detail
		* 値が大きいほどカメラが遅れて付いてきます。
		*/
		void SetDampingRate(float dampingRate)
		{
			m_targetDampingRate = dampingRate;
		}
		/*!
		* @brief	カメラを取得。
		*/
		CCamera* GetCamera() 
		{
			return &m_camera;
		}
		void SetViewAngle(float angle)
		{
			m_camera.SetViewAngle(angle);
		}
		float GetViewAngle() const
		{
			return m_camera.GetViewAngle();
		}
	private:
		CCamera		m_camera;						//!<カメラ。
		CVector3	m_target = CVector3::Zero;		//!<目標となる注視点。
		CVector3	m_position = CVector3::Zero;	//!<目標となる視点。
		CVector3	m_targetMoveSpeed = CVector3::Zero;		//!<注視点の移動速度。
		CVector3	m_positionMoveSpeed = CVector3::Zero;	//!<視点の移動速度。
		float		m_maxMoveSpeed = 0.0f;					//!<最高移動速度。
		float		m_targetDampingRate = 1.0f;				//!<減衰率。値が大きいほどカメラが遅れ付いてくる。
		float		m_dampingRate = 1.0f;					//!<減衰率。
		float		m_dampingRateVel = 0.0f;
	};
}