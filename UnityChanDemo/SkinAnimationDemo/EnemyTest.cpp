#include "stdafx.h"
#include "EnemyTest.h"
#include "Car.h"
#include "Map/Ground.h"

CSkinModelData*	EnemyTest::orgSkinModelData = NULL;	//�I���W�i���X�L�����f���f�[�^�B


namespace {
	CAnimation				*orgAnimation;			//�A�j���[�V�����B
	const float MAX_RUN_SPEED = 0.1f;					//���j�e�B�����̑���̍ō����x�B
	const float RUN_THREADHOLD_SQ = 0.07f * 0.07f;		//����A�j���[�V�������Đ����鑬�x��臒l�B
	//�n�ʂƂ̓����蔻��B
	struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit;
		CVector3 hitPos;
		CVector3 startPos;
		float dist;
		CVector3 hitNormal;
		SweepResultGround()
		{
			isHit = false;
			dist = FLT_MAX;
			hitPos = CVector3::Zero;
			startPos = CVector3::Zero;
			hitNormal = CVector3::Zero;
		}

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
			float t = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up)));
			if (t < CMath::PI * 0.3f) {
				isHit = true;
				CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
				//��_�Ƃ̋����𒲂ׂ�B
				CVector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					hitPos = hitPosTmp;
					dist = distTmp;
					hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
				}
			}
			return 0.0f;
		}
	};
	//��
	struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit;
		CVector3 hitPos;
		CVector3 startPos;
		float dist;
		CVector3 hitNormal;
		SweepResultWall()
		{
			isHit = false;
			dist = FLT_MAX;
			hitPos = CVector3::Zero;
			startPos = CVector3::Zero;
			hitNormal = CVector3::Zero;
		}

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
			float t = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up)));
			if (t >= CMath::PI * 0.3f) {
				isHit = true;
				CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
				//��_�Ƃ̋����𒲂ׂ�B
				CVector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					hitPos = hitPosTmp;
					dist = distTmp;
					hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
				}
			}
			return 0.0f;
		}
	};
}
/*!
* @brief	�J�n
*/
void EnemyTest::Start()
{
	if (orgSkinModelData == NULL) {
		orgSkinModelData = new CSkinModelData;
		orgAnimation = new CAnimation;
		//orgSkinModelData->LoadModelData("Assets/modelData/Unity.X", orgAnimation);
		orgSkinModelData->LoadModelData("Assets/modelData/player.X", orgAnimation);
	}
	//�I���W�i���̃��f���f�[�^����N���[�����f�����쐬�B
	skinModelData.CloneModelData(*orgSkinModelData, &animation);
	
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	skinModel.SetFresnelFlag(true);
	skinModel.SetReflectionCasterFlag(true);

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));


	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));

	light.SetLimLightColor(CVector4(0.6f, 0.6f, 0.6f, 1.0f));
	light.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	isPointLightOn = false;
	UpdatePointLightPosition();

	currentAnimSetNo = AnimationInvalid;
	PlayAnimation(currentAnimSetNo);
	rotation = CQuaternion::Identity;

	CVector3 lightPos = CVector3(0.0f, 25.5f, 24.5f);
	ShadowMap().SetLightPosition(lightPos);
	ShadowMap().SetLightTarget(position);
	toLightPos.Subtract(lightPos, position);
	ShadowMap().SetCalcLightViewFunc(CShadowMap::enCalcLightViewFunc_PositionTarget);
	state = enStateStand;
	isJump = false;

	radius = 0.4f;

	collider.Create(radius);
	RigidBodyInfo rbInfo;
	rbInfo.pos = position;
	rbInfo.rot = rotation;
	rbInfo.collider = &collider;
	rigidBody.Create(rbInfo);
	toLampLocalPos.Set( 0.0f, 0.5f, 0.2f);
	//g_physicsWorld->AddRigidBody(&rigidBody);
}
void EnemyTest::Update()
{
	CVector3 nextPosition = position;
	const float MOVE_SPEED = 5.0f;
	if (state == enStateRun || state == enStateStand) {
		if (Pad(0).IsTrigger(enButtonRB3)) {
			isPointLightOn = !isPointLightOn;
		}
		if (Pad(0).IsPress(enButtonA)) {
			//A�{�^���������ꂽ�B
			//�ԂƂ̋����𒲂ׂ�B
			CVector3 diff = g_car->GetPosition();
			diff.Subtract(position);
			if (diff.Length() < 2.0f) {
				//�ԂƂ̋�����2m�ȓ��B
				state = enState_RideOnCar;
				skinModel.SetShadowReceiverFlag(false);
				skinModel.SetShadowCasterFlag(false);
				g_car->SetRideOnFlag(true);
				g_camera->SetCar(g_car);
				return;
			}
			else if(!isJump){
				//�ԂƂ̋���������Ă�����W�����v�B
				moveSpeed.y = 8.0f;
				isJump = true;
			}
		}
		//���肩������Ԃ̎��B
		CVector3 moveDirLocal;
		moveDirLocal.y = 0.0f;
		moveDirLocal.x = Pad(0).GetLStickXF();
		moveDirLocal.z = Pad(0).GetLStickYF();
		const CMatrix& mViewInv = g_camera->GetCamera().GetViewMatrixInv();
		//�J������Ԃ��猩���������̃x�N�g�����擾�B
		CVector3 cameraZ;
		cameraZ.x = mViewInv.m[2][0];
		cameraZ.y = 0.0f;		//Y������Ȃ��B
		cameraZ.z = mViewInv.m[2][2];
		cameraZ.Normalize();	//Y����ł������Ă���̂Ő��K������B
								//�J�������猩���������̃x�N�g�����擾�B
		CVector3 cameraX;
		cameraX.x = mViewInv.m[0][0];
		cameraX.y = 0.0f;		//Y���͂���Ȃ��B
		cameraX.z = mViewInv.m[0][2];
		cameraX.Normalize();	//Y����ł������Ă���̂Ő��K������B

		CVector3 moveDir;
		moveDir.x = cameraX.x * moveDirLocal.x + cameraZ.x * moveDirLocal.z;
		moveDir.y = 0.0f;	//Y���͂���Ȃ��B
		moveDir.z = cameraX.z * moveDirLocal.x + cameraZ.z * moveDirLocal.z;

		moveSpeed.x = moveDir.x * MOVE_SPEED;
		moveSpeed.z = moveDir.z * MOVE_SPEED;
		
		//Y�����ɂ͏d�͗�����������B
		const float GRAVITY = -18.8f;
		moveSpeed.y += GRAVITY * GameTime().GetFrameDeltaTime();
		CVector3 addPos = moveSpeed;
		addPos.Scale(GameTime().GetFrameDeltaTime());
		nextPosition.Add(addPos);
		if (moveDir.LengthSq() > 0.0001f) {
			rotation.SetRotation(CVector3::Up, atan2f(moveDir.x, moveDir.z));
			//�����ԂɑJ�ځB
			state = enStateRun;
		}
		else {
			//������ԁB
			state = enStateStand;
		}
		
		ShadowMap().SetLightTarget(position);
		CVector3 lightPos;
		lightPos.Add(position, toLightPos);
		ShadowMap().SetLightPosition(lightPos);
		//�R���W�������o�Ɖ������s���B
		CollisionDetectAndResolve(nextPosition);
	}
	else if (state == enState_RideOnCar) {
		ShadowMap().SetLightTarget(g_car->GetPosition());
		CVector3 lightPos;
		lightPos.Add(g_car->GetPosition(), toLightPos);
		ShadowMap().SetLightPosition(lightPos);
		if (g_car->GetMoveSpeed().Length() < 0.1f) {
			//�Ԃ���~��ԁB
			if (Pad(0).IsPress(enButtonB)) {
				//�~�ԁB
				g_camera->SetCar(NULL);
				g_car->SetRideOnFlag(false);
				skinModel.SetShadowReceiverFlag(true);
				skinModel.SetShadowCasterFlag(true);
				position = g_car->GetPosition();
				state = enStateStand;
			}
		}
	}
	
	skinModel.Update(position, rotation, CVector3::One);
	
	//�|�C���g���C�g�̈ʒu���X�V�B
	UpdatePointLightPosition();
	//�A�j���[�V�����R���g���[���B
	AnimationControl();
	lastFrameState = state;
}
/*!
* @brief	�|�C���g���C�g�̈ʒu���X�V�B
*/
void EnemyTest::UpdatePointLightPosition()
{
	if (isPointLightOn) {
		pointLightColor.Set(0.9f, 0.75f, 0.6f, 1.0f);
	}
	else {
		pointLightColor = CVector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	light.SetPointLightColor(pointLightColor);
	pointLightPosition = toLampLocalPos;
	CMatrix mWorld = skinModel.GetWorldMatrix();
	mWorld.Mul(pointLightPosition);
	light.SetPointLightPosition(pointLightPosition);
}
/*!
* @brief	�Փˌ��o�Ɖ����B
*@param[in]	nextPosition		���̍��W�B
*/
void EnemyTest::CollisionDetectAndResolve(CVector3 nextPosition)
{
	//XZ���ʂ𒲂ׂ�B
	{
		int loopCount = 0;
		while (true) {
			CVector3 addPos;
			addPos.Subtract(nextPosition, position);
			CVector3 posTmp = position;
			posTmp.y += radius + 0.2f;
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(*(btVector3*)(&posTmp));
			CVector3 newPos;
			SweepResultWall callback;
			callback.startPos = position;
			CVector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() > 0.0001f) {
				newPos.Add(posTmp, addPosXZ);
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));

				PhysicsWorld().ConvexSweepTest((const btConvexShape*)collider.GetBody(), start, end, callback);
			}
			if (callback.isHit) {
				//���������B
				float t = fabsf(acosf(callback.hitNormal.Dot(CVector3::Up)));
				if (t >= CMath::PI * 0.3f) {
					//�ǁB
					nextPosition.x = callback.hitPos.x;
					nextPosition.z = callback.hitPos.z;
					//���a�������߂��B
					CVector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					CVector3 t = hitNormalXZ;
					t.Scale(radius);
					nextPosition.Add(t);
					//�����ĕǂɉ����Ċ��点��B
					t.Cross(hitNormalXZ, CVector3::Up);
					t.Normalize();
					//�����߂��œ��������͌��Z����B
					CVector3 t2;
					t2.Subtract(nextPosition, position);
					t2.y = 0.0f;
					addPosXZ.Subtract(t2);
					t.Scale(t.Dot(addPosXZ));
					nextPosition.Add(t);
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
	//�������𒲂ׂ�B
	{
		CVector3 addPos;
		addPos.Subtract(nextPosition, position);
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(position.x, position.y + radius, position.z));
		CVector3 newPos;
		SweepResultGround callback;
		callback.startPos = position;
		if (addPos.y < 0.0f) {
			newPos = (*(CVector3*)&start.getOrigin());
			newPos.y += addPos.y;

			end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));

			PhysicsWorld().ConvexSweepTest((const btConvexShape*)collider.GetBody(), start, end, callback);
		}
		if (callback.isHit) {
			//���������B
			float t = fabsf(acosf(callback.hitNormal.Dot(CVector3::Up)));
			if (t < CMath::PI * 0.3f) {
				//�n�ʁB
				CVector3 Circle;
				float x = 0.0f;
				float offset = 0.0f;	//�����߂��ʁB
				Circle = CVector3::Zero;

				Circle = position;
				Circle.y = callback.hitPos.y;//�~�̒��S
				CVector3 v;
				v.Subtract(Circle, callback.hitPos);
				x = v.Length();//���̂̊p�ƃv���C���[�̊Ԃ̉����̋��������܂�B

				offset = sqrt(max(0.0f, radius*radius - x*x));//y�̕����������߂�B

				moveSpeed.y = 0.0f;
				isJump = false;
				nextPosition.y = callback.hitPos.y + offset - radius;
			}
		}
	}
	position = nextPosition;
}
/*!
* @brief	�A�j���[�V�����Đ��B
*/
void EnemyTest::PlayAnimation(AnimationNo animNo)
{
	if (currentAnimSetNo != animNo) {
		//�ʂ̃A�j���[�V����
		animation.PlayAnimation(animNo, 0.3f);
		currentAnimSetNo = animNo;
	}
}
/*!
* @brief	�A�j���[�V�����R���g���[���B
*/
void EnemyTest::AnimationControl()
{
	animation.Update(GameTime().GetFrameDeltaTime());
	if (Pad(0).IsPress(enButtonB)) {
		PlayAnimation(AnimationAttack);
	}else{
		if (state == enStateRun) {
			//�����A�j���[�V�����B
			PlayAnimation(AnimationWalk);
		}
		else if (state == enStateStand) {
			//�����A�j���[�V�����𗬂��B
			PlayAnimation(AnimationStand);
		}
	}
}
/*!
* @brief	�`��B
*/
void EnemyTest::Render(CRenderContext& renderContext)
{
	if (state != enState_RideOnCar) {
		//�Ԃɏ���Ă���Ƃ��͔�\���ɂ���B
		skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
	}
}
