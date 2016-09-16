#include "stdafx.h"
#include "UnityChan.h"
#include "Car.h"
#include "Ground.h"

CSkinModelData*	UnityChan::orgSkinModelData = NULL;	//�I���W�i���X�L�����f���f�[�^�B
CAnimation				*orgAnimation;			//�A�j���[�V�����B

namespace {
	const float MAX_RUN_SPEED = 0.1f;					//���j�e�B�����̑���̍ō����x�B
	const float RUN_THREADHOLD_SQ = 0.07f * 0.07f;		//����A�j���[�V�������Đ����鑬�x��臒l�B
}
/*!
* @brief	�J�n
*/
void UnityChan::Start()
{
	if (orgSkinModelData == NULL) {
		orgSkinModelData = new CSkinModelData;
		orgAnimation = new CAnimation;
		orgSkinModelData->LoadModelData("Assets/modelData/Unity.X", orgAnimation);
	}
	//�I���W�i���̃��f���f�[�^����N���[�����f�����쐬�B
	skinModelData.CloneModelData(*orgSkinModelData, &animation);
	normalMap.Load("Assets/modelData/utc_nomal.tga");
	specMap.Load("Assets/modelData/utc_spec.tga");
	//skinModelData.LoadModelData("Assets/modelData/unity.X", NULL);
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	skinModel.SetNormalMap(&normalMap);
	skinModel.SetSpeculerMap(&specMap);
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

	animation.SetAnimationEndTime(AnimationRun, 0.8);
	currentAnimSetNo = AnimationInvalid;
	PlayAnimation(currentAnimSetNo);
	rotation = CQuaternion::Identity;

	CVector3 lightPos = CVector3(0.0f, 9.5f, 8.5f);
	ShadowMap().SetLightPosition(lightPos);
	ShadowMap().SetLightTarget(position);
	toLightPos.Subtract(lightPos, position);
	ShadowMap().SetCalcLightViewFunc(CShadowMap::enCalcLightViewFunc_PositionTarget);
	state = enStateStand;
	isJump = false;
}
void UnityChan::Update()
{
	CVector3 nextPosition = position;
	const float MOVE_SPEED = 5.0f;
	if (state == enStateRun || state == enStateStand) {
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
		moveSpeed.y += GRAVITY * 1.0f/60.0f;
		CVector3 addPos = moveSpeed;
		addPos.Scale(1.0f / 60.0f);
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
	
	
	//�A�j���[�V�����R���g���[���B
	AnimationControl();
	lastFrameState = state;
}
/*!
* @brief	�Փˌ��o�Ɖ����B
*@param[in]	nextPosition		���̍��W�B
*/
void UnityChan::CollisionDetectAndResolve(const CVector3& nextPosition)
{
	//XY���ʂƂ̏ՓˁB
	position.x = nextPosition.x;
	position.z = nextPosition.z;

	//�n�ʂƂ̓����蔻��B��͌��Ȃ���B
	int isHit = false;
	float len = 0.0f;
	CVector3 ray;
	ray.Subtract(nextPosition, position);
	float fallSpeed = ray.Length();
	if (fallSpeed > 0.0f) {
		ray.Normalize();
		if (ray.y < 0.0f) {
			//�������Ȃ�
			CVector3 start = position;
			start.y += 0.5f;	//���_�͂�����Əォ��B
			g_ground->IsIntersect(start, ray, isHit, len);
			if (isHit && len-0.5f < fallSpeed) {
				ray.Scale(len-0.5f);
				position.Add(ray);
				//�n�ʂɗ������B
				moveSpeed.y = 0.0f;
				isJump = false;
			}else{
				isHit = false;
			}
		}
		if (!isHit) {
			//�Փ˂��Ă��Ȃ��B
			position.y = nextPosition.y;
		}
	}
	
}
/*!
* @brief	�A�j���[�V�����Đ��B
*/
void UnityChan::PlayAnimation(AnimationNo animNo)
{
	if (currentAnimSetNo != animNo) {
		//�ʂ̃A�j���[�V����
		animation.PlayAnimation(animNo, 0.1f);
		currentAnimSetNo = animNo;
	}
}
/*!
* @brief	�A�j���[�V�����R���g���[���B
*/
void UnityChan::AnimationControl()
{
	animation.Update(1.0f / 60.0f);
	if (isJump) {
		PlayAnimation(AnimationJump);
	}else{
		if (state == enStateRun) {
			if (moveSpeed.LengthSq() > RUN_THREADHOLD_SQ) {
				//����A�j���[�V�����𗬂��B
				PlayAnimation(AnimationRun);
			}
			else {
				//�����A�j���[�V�����B
				PlayAnimation(AnimationWalk);
			}
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
void UnityChan::Render(CRenderContext& renderContext)
{
	if (state != enState_RideOnCar) {
		//�Ԃɏ���Ă���Ƃ��͔�\���ɂ���B
		skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
	}
}
