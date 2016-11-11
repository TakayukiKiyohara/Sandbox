/*!
 * @brief	バネカメラ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/camera/tkSpringCamera.h"

namespace tkEngine{
	namespace {
		float dampingK = 35.0f;
		float CalcSpringScalar(
			float positionNow,
			float positionTarget,
			float& moveSpeed)
		{
			float deltaTime = min(1.0f/30.0f,GameTime().GetFrameDeltaTime());
			
			float dampingRate = 0.2f;
			float distance;
			distance = positionTarget - positionNow;
			if (fabsf(distance) < FLT_EPSILON) {
				moveSpeed = 0.0f;
				return positionTarget;
			}
			float originalDir = distance;
			originalDir /= fabsf(distance);
			float springAccel;
			springAccel = distance;

			float t = dampingK / (2.0f * dampingRate);
			float springK = t * t;
			springAccel *= springK;
			//加速度を決定。
			float vt = moveSpeed;
			vt *= dampingK;
			springAccel -= vt;
			springAccel *= deltaTime;
			moveSpeed += springAccel;
			
			float newPos = positionNow;
			float addPos = moveSpeed;
			addPos *= deltaTime;
			newPos += addPos;
			vt = positionTarget - newPos;
			if (fabsf(vt) < FLT_EPSILON) {
				//目標座標まで移動完了した。
				newPos = positionTarget;
				moveSpeed = 0.0f;
			}
			else {
				vt /= fabsf(vt);
				if (vt * originalDir < 0.0f) {
					//目標座標を超えた。
					newPos = positionTarget;
					moveSpeed = 0.0f;
				}
			}
			return newPos;
		}
		/*!
		* @brief	バネ減衰を使用して、現在の位置、目標となる位置、速度、加速度から新しい位置を計算する。
		*/
		CVector3 CalcSpringVector( 
			const CVector3& positionNow, 
			const CVector3& positionTarget, 
			CVector3& moveSpeed ,
			float maxMoveSpeed,
			float dampingRate
		)
		{
			float deltaTime = min(1.0f / 30.0f, GameTime().GetFrameDeltaTime());
			//現在の位置と目標の位置との差分を求める。
			CVector3 distance;
			distance.Subtract( positionTarget, positionNow );
			CVector3 originalDir = distance;
			originalDir.Normalize();
			CVector3 springAccel;
			springAccel = distance;
			
			float t = dampingK / (2.0f * dampingRate);
			float springK = t * t;
			springAccel.Scale(springK);
			//加速度を決定。
			CVector3 vt = moveSpeed;
			vt.Scale(dampingK);
			springAccel.Subtract(vt);
			
			springAccel.Scale(deltaTime);
			moveSpeed.Add(springAccel);
			if (moveSpeed.LengthSq() > maxMoveSpeed*maxMoveSpeed) {
				//最高速度より速くなってしまった。
				moveSpeed.Normalize();
				moveSpeed.Scale(maxMoveSpeed);
			}
			CVector3 newPos = positionNow;
			CVector3 addPos = moveSpeed;
			addPos.Scale(deltaTime);
			newPos.Add(addPos);
			vt.Subtract(positionTarget, newPos);
			vt.Normalize();
			if (vt.Dot(originalDir) < 0.0f) {
				//目標座標を超えた。
				newPos = positionTarget;
				moveSpeed = CVector3::Zero;
			}
			return newPos;
		}
	}
	/*!
	 * @brief	コンストラクタ。
	 */
	CSpringCamera::CSpringCamera()
	{
	}
	/*!
	 * @brief	デストラクタ
	 */
	CSpringCamera::~CSpringCamera()
	{
	}
	/*!
	* @brief	初期化。
	*/
	void CSpringCamera::Init(const CVector3& target, const CVector3& position, float maxMoveSpeed)
	{
		m_camera.SetTarget(target);
		m_camera.SetPosition(position);
		m_target = target;
		m_position = position;
		m_targetMoveSpeed = CVector3::Zero;
		m_positionMoveSpeed = CVector3::Zero;
		m_maxMoveSpeed = maxMoveSpeed;
	}
	/*!
	 * @brief	更新。
	 */
	void CSpringCamera::Update()
	{
		m_dampingRate = CalcSpringScalar(m_targetDampingRate, m_dampingRate, m_dampingRateVel);
		CVector3 target = CalcSpringVector(m_camera.GetTarget(), m_target, m_targetMoveSpeed, m_maxMoveSpeed, m_dampingRate);
		CVector3 position = CalcSpringVector(m_camera.GetPosition(), m_position,  m_positionMoveSpeed, m_maxMoveSpeed, m_dampingRate);
		m_camera.SetTarget(target);
		
		m_camera.SetPosition(position);
		m_camera.Update();
	}
}