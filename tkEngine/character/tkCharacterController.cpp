/*!
* @brief	�L�����N�^�̃R���W�����R���g���[���B
*/

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/character/tkCharacterController.h"
#include "tkEngine/character/tkCollisionAttr.h"
namespace tkEngine {

	namespace {
		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;									//�Փ˃t���O�B
			CVector3 hitPos = CVector3(0.0f, -FLT_MAX, 0.0f);	//�Փ˓_�B
			CVector3 startPos = CVector3::Zero;					//���C�̎n�_�B
			CVector3 hitNormal = CVector3::Zero;				//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
			float dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B

																//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
					) {
					//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
				//������Ɩ@���̂Ȃ��p�x�����߂�B
				float angle = hitNormalTmp.Dot(CVector3::Up);
				angle = fabsf(acosf(angle));
				if (angle < CMath::PI * 0.3f		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //�������̓R���W�����������n�ʂƎw�肳��Ă���B
					) {
					//�Փ˂��Ă���B
					isHit = true;
					CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
					//�Փ˓_�̋��������߂�B�B
					CVector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					if (dist > distTmp) {
						//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
						hitPos = hitPosTmp;
						hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
						dist = distTmp;
					}
				}
				return 0.0f;
			}
		};
		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
		struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//�Փ˃t���O�B
			CVector3 hitPos = CVector3::Zero;		//�Փ˓_�B
			CVector3 startPos = CVector3::Zero;		//���C�̎n�_�B
			float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
			CVector3 hitNormal = CVector3::Zero;	//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
													//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me) {
					//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				CVector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
				float angle = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up)));
				if (angle >= CMath::PI * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//�������̓R���W�����������L�����N�^�Ȃ̂ŕǂƂ݂Ȃ��B
					) {
					isHit = true;
					CVector3 hitPosTmp;
					hitPosTmp.Set(convexResult.m_hitPointLocal);
					//��_�Ƃ̋����𒲂ׂ�B
					CVector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					vDist.y = 0.0f;
					float distTmp = vDist.Length();
					if (distTmp < dist) {
						//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
						hitPos = hitPosTmp;
						dist = distTmp;
						hitNormal = hitNormalTmp;
					}
				}
				return 0.0f;
			}
		};
	}


	void CCharacterController::Init(float radius, float height, const CVector3& position)
	{
		m_position = position;
		//�R���W�����쐬�B
		m_radius = radius;
		m_height = height;
		m_collider.Create(radius, height);

		//���̂��������B
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Create(rbInfo);
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		//@todo ���Ή��Btrans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		PhysicsWorld().AddRigidBody(&m_rigidBody);

	}
	void CCharacterController::Execute()
	{
		//���x�ɏd�͉����x��������B
		m_moveSpeed.y += m_gravity * GameTime().GetFrameDeltaTime();
		//���̈ړ���ƂȂ���W���v�Z����B
		CVector3 nextPosition = m_position;
		//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
		CVector3 addPos = m_moveSpeed;
		addPos.Scale(GameTime().GetFrameDeltaTime());
		nextPosition.Add(addPos);
		CVector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
		{
			int loopCount = 0;
			while (true) {
				//���݂̍��W���玟�̈ړ���֌������x�N�g�������߂�B
				CVector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				CVector3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (addPosXZ.Length() < FLT_EPSILON) {
					//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
					//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
					//�ƂĂ��������l�̂��Ƃł��B
					break;
				}
				//�J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
				CVector3 posTmp = m_position;
				posTmp.y += m_height * 0.5f + m_radius + 0.2f;
				//���C���쐬�B
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//�I�_�͎��̈ړ���BXZ���ʂł̏Փ˂𒲂ׂ�̂ŁAy��posTmp.y��ݒ肷��B
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				SweepResultWall callback;
				callback.me = m_rigidBody.GetBody();
				callback.startPos = posTmp;
				//�Փˌ��o�B
				PhysicsWorld().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

				if (callback.isHit) {
					//���������B
					//�ǁB
					CVector3 vT0, vT1;
					//XZ���ʏ�ł̈ړ���̍��W��vT0�ɁA��_�̍��W��vT1�ɐݒ肷��B
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
					//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
					CVector3 vMerikomi;
					vMerikomi.Subtract(vT0, vT1);
					//XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
					CVector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//�����߂��Ԃ��x�N�g�������߂�B
					//�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
					CVector3 vOffset;
					vOffset = hitNormalXZ;
					vOffset.Scale(-fT0 + m_radius);
					nextPosition.Add(vOffset);
					CVector3 currentDir;
					currentDir.Subtract(nextPosition, m_position);
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//�p�ɓ��������̃L�����N�^�̐U����h�~���邽�߂ɁA
						//�ړ��悪�t�����ɂȂ�����ړ����L�����Z������B
						nextPosition.x = m_position.x;
						nextPosition.z = m_position.z;
						break;
					}
				}
				else {
					//�ǂ��Ƃ�������Ȃ��̂ŏI���B
					break;
				}
				loopCount++;
				if (loopCount == 5) {
					break;
				}
			}
		}
		//XZ�̈ړ��͊m��B
		m_position.x = nextPosition.x;
		m_position.z = nextPosition.z;
		//�������𒲂ׂ�B
		{
			CVector3 addPos;
			addPos.Subtract(nextPosition, m_position);

			m_position = nextPosition;	//�ړ��̉��m��B
										//���C���쐬����B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
			start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
			//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
			//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
			//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
			CVector3 endPos;
			endPos.Set(start.getOrigin());
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//�W�����v���Ƃ��ŏ㏸���B
					//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������̂ŉ��𒲂ׂ�B
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
					endPos.y += addPos.y;
				}
			}
			else {
				//�n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
				endPos.y -= 1.0f;
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			SweepResultGround callback;
			callback.me = m_rigidBody.GetBody();
			callback.startPos.Set(start.getOrigin());
			//�Փˌ��o�B
			PhysicsWorld().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.isHit) {
				//���������B
			/*	CVector3 Circle;
				float x = 0.0f;
				float offset = 0.0f;	//�����߂��ʁB
				Circle = CVector3::Zero;

				Circle = m_position;
				Circle.y = callback.hitPos.y;//�~�̒��S
				CVector3 v;
				v.Subtract(Circle, callback.hitPos);
				x = v.Length();//���̂̊p�ƃv���C���[�̊Ԃ̉����̋��������܂�B

				offset = sqrt(max(0.0f, m_radius*m_radius - x*x));//y�̕����������߂�B
				*/
				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;//; + offset - m_radius;
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B
				m_isOnGround = false;
			}
		}
		//�ړ��m��B
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//���̂𓮂����B
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
		//@todo ���Ή��B trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	}
	/*!
	* @brief	���S�������Ƃ�ʒm�B
	*/
	void CCharacterController::RemoveRigidBoby()
	{
		PhysicsWorld().RemoveRigidBody(&m_rigidBody);
	}
}
