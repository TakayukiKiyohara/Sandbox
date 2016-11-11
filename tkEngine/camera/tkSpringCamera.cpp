/*!
 * @brief	�o�l�J�����B
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
			//�����x������B
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
				//�ڕW���W�܂ňړ����������B
				newPos = positionTarget;
				moveSpeed = 0.0f;
			}
			else {
				vt /= fabsf(vt);
				if (vt * originalDir < 0.0f) {
					//�ڕW���W�𒴂����B
					newPos = positionTarget;
					moveSpeed = 0.0f;
				}
			}
			return newPos;
		}
		/*!
		* @brief	�o�l�������g�p���āA���݂̈ʒu�A�ڕW�ƂȂ�ʒu�A���x�A�����x����V�����ʒu���v�Z����B
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
			//���݂̈ʒu�ƖڕW�̈ʒu�Ƃ̍��������߂�B
			CVector3 distance;
			distance.Subtract( positionTarget, positionNow );
			CVector3 originalDir = distance;
			originalDir.Normalize();
			CVector3 springAccel;
			springAccel = distance;
			
			float t = dampingK / (2.0f * dampingRate);
			float springK = t * t;
			springAccel.Scale(springK);
			//�����x������B
			CVector3 vt = moveSpeed;
			vt.Scale(dampingK);
			springAccel.Subtract(vt);
			
			springAccel.Scale(deltaTime);
			moveSpeed.Add(springAccel);
			if (moveSpeed.LengthSq() > maxMoveSpeed*maxMoveSpeed) {
				//�ō����x��葬���Ȃ��Ă��܂����B
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
				//�ڕW���W�𒴂����B
				newPos = positionTarget;
				moveSpeed = CVector3::Zero;
			}
			return newPos;
		}
	}
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CSpringCamera::CSpringCamera()
	{
	}
	/*!
	 * @brief	�f�X�g���N�^
	 */
	CSpringCamera::~CSpringCamera()
	{
	}
	/*!
	* @brief	�������B
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
	 * @brief	�X�V�B
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