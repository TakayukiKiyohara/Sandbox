#include "stdafx.h"
#include "Player.h"
#include "Car.h"
#include "Map/Ground.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"
#include "DamageCollisionWorld.h"
#include "ParticleParam.h"
#include "Enemy/EnemyParameter.h"
#include "tkEngine/Sound/tkSoundSource.h"

namespace {
	const float USE_MP_DASH = 50.0f;		//�_�b�V�����@���g��������MP�g�p�ʁB
	const float RECOVER_MP = 20.0f;			//MP���R�񕜗ʁB

	const float RUN_THREADHOLD_SQ = 4.0f * 4.0f;		//����A�j���[�V�������Đ����鑬�x��臒l�B
	//�A�j���[�V�����̃C�x���g�e�[�u���B
	AnimationEventGroup animationEventGroupTbl[Player::NumAnimation] = {
		//AnimationStand
		{
			END_ANIMATION_EVENT(),
		},
		//AnimationWalk
		{
			END_ANIMATION_EVENT(),
		},
		//AnimationRun
		{
			EMIT_SOUND_EVENT(0.4f, 1.0f, "Assets/sound/Footstep_00.wav"),
			EMIT_SOUND_EVENT(0.9f, 1.0f, "Assets/sound/Footstep_00.wav"),
			END_ANIMATION_EVENT(),
		},
		//AnimationJump
		{
			END_ANIMATION_EVENT(),
		},
		//AnimationAttack_00
		{
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(0.4f, 0.3f, 0.7f, 10, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 0),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(0.48f, 0.3f, 0.7f, 10, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 0),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(0.56f, 0.3f, 0.7f, 10, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 0),
			EMIT_SOUND_EVENT(0.3f, 0.5f, "Assets/sound/PlayerAttack_00.wav"),
			END_ANIMATION_EVENT(),
		},
		//AnimationAttack_01
		{
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(0.4f, 0.3f, 0.7f, 10, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 1),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(0.48f, 0.3f, 0.7f, 10, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 1),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(0.56f, 0.3f, 0.7f, 10, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 1),
			EMIT_SOUND_EVENT(0.3f, 0.5f, "Assets/sound/PlayerAttack_00.wav"),
			END_ANIMATION_EVENT(),
		},
		//AnimationAttack_02
		{
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.3f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.31f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.32f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.33f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.34f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.35f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.36f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.37f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.38f, 0.3f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.39f, 0.35f, 0.7f, 20, "thief_Bip01_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),

			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.3f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.31f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.32f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.33f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.34f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.35f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.36f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.37f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.38f, 0.3f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(1.39f, 0.35f, 0.7f, 20, "thief_Bip01_L_Hand", CVector3(0.0f, 0.0f, 0.0f), 2),
			EMIT_SOUND_EVENT(1.2f, 0.5f, "Assets/sound/PlayerAttack_01.wav"),
			END_ANIMATION_EVENT(),
		},
		//AnimationDamage
		{
			END_ANIMATION_EVENT(),
		},
		//AnimationDeath
		{
			EMIT_SOUND_EVENT(0.0f, 0.75f, "Assets/sound/PlayerDead.wav"),
			END_ANIMATION_EVENT(),
		},
	};
}
AnimationEvent hoge[] = {
	END_ANIMATION_EVENT(),
	END_ANIMATION_EVENT(),
};
/*!
* @brief	�J�n
*/
void Player::Start()
{
	SkinModelDataResources().Load(skinModelData, "Assets/modelData/Player.X", &animation);
	normalMap.Load("Assets/modelData/Thethief_N.tga");
	specMap.Load("Assets/modelData/Thethief_S.tga");
	weaponNormalMap.Load("Assets/modelData/Thethief_wuqi_N.tga");
	weaponSpecMap.Load("Assets/modelData/Thethief_wuqi_S.tga");
	//�̂̃}�e���A�����擾�B
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Thethief_D.tga");
	if (mat != NULL) {
		mat->SetTexture("g_normalTexture", &normalMap);
		mat->SetTexture("g_speculerMap", &specMap);
	}
	//����̃}�e���A�����擾�B
	mat = skinModelData.GetBody()->FindMaterial("Thethief_wuqi_D.tga");
	if (mat != NULL) {
		mat->SetTexture("g_normalTexture", &weaponNormalMap);
		mat->SetTexture("g_speculerMap", &weaponSpecMap);
	}
	skinModel.Init(skinModelData.GetBody());
	skinModel.SetLight(&light);
	skinModel.SetHasNormalMap(true);
	skinModel.SetHasSpeculerMap(true);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	skinModel.SetFresnelFlag(true);
	skinModel.SetReflectionCasterFlag(true);
	skinModel.SetWriteVelocityMap(false);

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));


	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 20.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 20.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 20.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 20.0f));
	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));

	light.SetLimLightColor(CVector4(0.6f, 0.6f, 0.6f, 1.0f));
	light.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	isPointLightOn = false;
	UpdatePointLightPosition();

	PlayAnimation(AnimationInvalid, 0.0f);
	rotation = CQuaternion::Identity;
	
	CVector3 lightPos = CVector3(0.0f, 25.5f, 24.5f);
	ShadowMap().SetLightPosition(lightPos);
	ShadowMap().SetLightTarget(position);
	toLightPos.Subtract(lightPos, position);
	ShadowMap().SetCalcLightViewFunc(CShadowMap::enCalcLightViewFunc_PositionTarget);
	state = enStateStand;
	radius = 0.4f;
	height = 0.3f;
	characterController.Init(radius, height, position);
	characterController.SetGravity(-18.8f);
	toLampLocalPos.Set( 0.0f, 0.5f, 0.2f);
	InitBattleSeats();
	//g_physicsWorld->AddRigidBody(&rigidBody);
	animation.SetAnimationEndTime(AnimationAttack_00, 0.63333f);
	animation.SetAnimationEndTime(AnimationAttack_01, 0.76666f);
	animation.SetAnimationEndTime(AnimationDamage, 0.733333f);
	animation.SetAnimationLoopFlag(AnimationJump, false);
	animation.SetAnimationLoopFlag(AnimationAttack_00, false);
	animation.SetAnimationLoopFlag(AnimationAttack_01, false);
	animation.SetAnimationLoopFlag(AnimationAttack_02, false);
	animation.SetAnimationLoopFlag(AnimationDamage, false);
	animation.SetAnimationLoopFlag(AnimationDeath, false);
	nextAttackAnimNo = AnimationInvalid;
	reqAttackAnimNo = AnimationInvalid;

	//�A�j���[�V�����C�x���g�R���g���[���̏������B
	animationEventController.Init(
		&skinModel, 
		&animation, 
		animationEventGroupTbl, 
		sizeof(animationEventGroupTbl)/sizeof(animationEventGroupTbl[0])
	);
}
void Player::UpdateStateMachine()
{
	const float MOVE_SPEED = 7.0f;
	if (state == enStateRun || state == enStateStand) {
		CVector3 moveSpeed = characterController.GetMoveSpeed();
		if (Pad(0).IsTrigger(enButtonRB3)) {
			isPointLightOn = !isPointLightOn;
		}
		if (Pad(0).IsPress(enButtonA)) {
			//A�{�^���������ꂽ�B
			//�ԂƂ̋����𒲂ׂ�B
			CVector3 diff = g_car->GetPosition();
			diff.Subtract(position);
			if (!characterController.IsJump()) {
				//�W�����v�B
				moveSpeed.y = 8.0f;
				characterController.Jump();
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

		float fMoveSpeed = MOVE_SPEED;
		bool isDash = false;
		if (Pad(0).IsPress(enButtonRB2)) {
			//MP�̎c�ʂ����ă_�b�V���ł��邩���ׂ�B
			float useMp = USE_MP_DASH * GameTime().GetFrameDeltaTime();
			if (mp - useMp >= 0.0f) {
				//MP�g����B
				isDash = true;
			}
			UseMagicPoint(useMp);
		}

		if (isDash) {
			//�_�b�V�����\�����ׂ�B
			fMoveSpeed *= 1.7f;
			g_camera->SetViewAngle(CMath::DegToRad(60.0f));
			g_camera->SetDampingRate(0.9f);
			animation.SetAnimationSpeedRate(1.4f);
			MotionBlur().SetEnable(true);
		}
		else {
			g_camera->SetViewAngle(CMath::DegToRad(70.0f));
			g_camera->SetDampingRate(0.9f);
			animation.SetAnimationSpeedRate(1.0f);
			MotionBlur().SetEnable(false);
		}
		moveSpeed.x = moveDir.x * fMoveSpeed;
		moveSpeed.z = moveDir.z * fMoveSpeed;

		if (moveDir.LengthSq() > 0.0001f) {
			rotation.SetRotation(CVector3::Up, atan2f(moveDir.x, moveDir.z));
			//�����ԂɑJ�ځB
			ChangeState(enStateRun);
		}
		else {
			//������ԁB
			ChangeState(enStateStand);
		}
		bool isOnGround = characterController.IsOnGround();
		characterController.SetMoveSpeed(moveSpeed);
		characterController.Execute();
		if (isOnGround == false
			&& characterController.IsOnGround()
			) {
			//���n�����B
			//���n�����Đ��B
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init("Assets/sound/landing.wav");
			se->Play(false);
			se->SetVolume(0.25f);
		}

		if (Pad(0).IsTrigger(enButtonX) && !characterController.IsJump()) {
			nextAttackAnimNo = AnimationAttack_00;
			ChangeState(enState_Attack);
		}
	}

	else if (state == enState_Attack) {
		//�ړ����s�^���Ď~�܂�ƋC������邢�̂�
		CVector3 moveSpeed = characterController.GetMoveSpeed();
		moveSpeed.Scale(0.8f);
		characterController.SetMoveSpeed(moveSpeed);
		characterController.Execute();
		int currentAnimNo = animation.GetPlayAnimNo();
		if (!animation.IsPlay() && nextAttackAnimNo == AnimationInvalid) {
			ChangeState(enStateStand);
		}
		else if (
			Pad(0).IsTrigger(enButtonX)
			&& currentAnimNo >= AnimationAttack_Start
			&& currentAnimNo < AnimationAttack_End
			&& currentAnimNo == reqAttackAnimNo
			) {
			//�R���{�����B
			nextAttackAnimNo = (AnimationNo)(animation.GetPlayAnimNo() + 1);
		}
	}
	else if (state == enState_Damage) {
		if (!animation.IsPlay()) {
			ChangeState(enStateStand);
		}
	}
	else if (state == enState_Dead) {
		timer += GameTime().GetFrameDeltaTime();
		if (timer > 1.0f) {
			//�G�~�b�^�[���폜
			for (auto& p : particleEmitterList) {
				DeleteGO(p);
			}
			particleEmitterList.clear();
		}
	}
	position = characterController.GetPosition();
}

/*!
* @brief	�����������B
*/
void Player::Damage()
{
	isApplyDamageTrigger = false;
	if (state == enState_Damage || state == enState_Dead) {
		return;
	}
	CRigidBody* rb = characterController.GetRigidBody();
	const DamageCollisionWorld::Collision* dmgColli = g_damageCollisionWorld->FindOverlappedDamageCollision(
		DamageCollisionWorld::enDamageToPlayer,
		rb->GetBody()
	);
	if (dmgColli != NULL) {
		//�_���[�W���󂯂��B
		hp -= dmgColli->damage;
		if (hp <= 0.0f) {
			//���S
			hp = 0;
			ChangeState(enState_Dead);
			isApplyDamageTrigger = true;
		}
		else {
			ChangeState(enState_Damage);
			isApplyDamageTrigger = true;
		}
	}
	
}
/*!
* @brief	�|�C���g���C�g�̈ʒu���X�V�B
*/
void Player::UpdatePointLightPosition()
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
* @brief	�A�j���[�V�����Đ��B
*/
void Player::PlayAnimation(AnimationNo animNo, float interpolateTime)
{
	if (animation.GetPlayAnimNo() != animNo && animNo != AnimationInvalid) {
		//�ʂ̃A�j���[�V����
		animation.PlayAnimation(animNo, interpolateTime);
	}
}
/*!
* @brief	�A�j���[�V�����R���g���[���B
*/
void Player::AnimationControl()
{
	if (characterController.IsJump()) {
		PlayAnimation(AnimationJump, 0.05f);
	}else{
		if (state == enStateRun) {
			if (characterController.GetMoveSpeed().LengthSq() > RUN_THREADHOLD_SQ) {
				//����A�j���[�V�����𗬂��B
				PlayAnimation(AnimationRun, 0.1f);
			}
			else {
				//�����A�j���[�V�����B
				PlayAnimation(AnimationWalk, 0.1f);
			}
		}
		else if (state == enStateStand) {
			//�����A�j���[�V�����𗬂��B
			PlayAnimation(AnimationStand, 0.3f);
		}
		else if (state == enState_Attack) {
			if (nextAttackAnimNo == AnimationAttack_Start) {
				//�U���J�n�B
				PlayAnimation(nextAttackAnimNo, 0.1f);
				reqAttackAnimNo = nextAttackAnimNo;
				nextAttackAnimNo = AnimationInvalid;
			}else if( nextAttackAnimNo != AnimationInvalid ){
				//�A���̃A�j���[�V���������N�G�X�g�L���[�ɐςށB
				animation.PlayAnimationQueue(nextAttackAnimNo, 0.05f);
				reqAttackAnimNo = nextAttackAnimNo;
				nextAttackAnimNo = AnimationInvalid;
			}
		}
		else if (state == enState_Damage) {
			if (isApplyDamageTrigger) {
				//�ʂ̃A�j���[�V����
				animation.PlayAnimation(AnimationDamage, 0.1f);
			}
		}
		else if (state == enState_Dead) {
			PlayAnimation(AnimationDeath, 0.1f);
		}
	}
	animation.Update(GameTime().GetFrameDeltaTime());
}
/*!
* @brief	�o�g���Ŏg�p����V�[�g���������B
*/
void Player::InitBattleSeats()
{
	float angleBase = 2.0f * CMath::PI / NUM_BATTLE_SEAT;
	for (int i = 0; i < NUM_BATTLE_SEAT; i++) {
		battleSeats[i].seatNo = i;
		battleSeats[i].isUse = false;
		battleSeats[i].localPosition.x = sinf(angleBase * i) * 1.5f;
		battleSeats[i].localPosition.y = 0.0f;
		battleSeats[i].localPosition.z = cosf(angleBase * i) * 1.5f;
		battleSeats[i].position.Add(battleSeats[i].localPosition, position);
	}
}
/*!
* @brief	�o�g���Ŏg�p����V�[�g���X�V�B
*/
void Player::UpdateBattleSeats()
{
	for (auto& seat : battleSeats) {
		seat.position.Add(seat.localPosition, position);
	}
}
/*!
* @brief	���g�p�̃V�[�g�������B
*@param[in]	pos		���g�p�̃V�[�g�̒����炱�̍��W�Ɉ�ԋ߂��V�[�g��Ԃ��܂��B
*@return		���g�p�̃V�[�g���Ȃ��ꍇ��NULL���Ԃ��Ă���B
*/
Player::SBattleSeat* Player::FindUnuseSeat(const CVector3& pos)
{
	float dist = FLT_MAX;
	float distTmp;
	SBattleSeat* result = NULL;
	for (auto& seat : battleSeats) {
		if (!seat.isUse) {
			//���g�p�B
			CVector3 diff;
			diff.Subtract(seat.position, pos);
			distTmp = diff.LengthSq();
			if (distTmp < dist) {
				dist = distTmp;
				result = &seat;
			}
		}
	}
	return result;
}
/*!
* @brief	��Ԑ؂�ւ��B
*/
void Player::ChangeState(EnState nextState)
{
	if (state == enState_Damage
		|| state == enState_Dead
	) {
		//���݂̃X�e�[�g���_���[�W��Ԃ̏ꍇ�B
		//�����Ԃ��̃p�[�e�B�N���G�~�b�^�[���폜�B
		for (auto& p : particleEmitterList) {
			DeleteGO(p);
		}
		particleEmitterList.clear();
	}
	state = nextState;
	if (state == enState_Damage
		|| state == enState_Dead)
	{
		//���̃X�e�[�g���_���[�W���or���S��Ԃ̏ꍇ�B
		//�����Ԃ��̃G�t�F�N�g���G�~�b�g�B
		CMatrix* m = skinModel.FindBoneWorldMatrix("thief_Bip01_Neck");
		for (SParicleEmitParameter& param : bloodEmitterParam) {
			CParticleEmitter* particleEmitter = NewGO<CParticleEmitter>(1);
			CVector3 pos;
			pos.Set(m->m[3][0], m->m[3][1], m->m[3][2]);
			particleEmitter->Init(g_random, g_camera->GetCamera(), param, pos);
			particleEmitterList.push_back(particleEmitter);
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init("Assets/sound/Damage_01.wav");
			se->Play(false);
			se->SetVolume(0.5f);
		}
	}
	if (state == enState_Dead) {
		timer = 0.0f;
	}
	
}
/*!
* @brief	�`��B
*/
void Player::Render(CRenderContext& renderContext)
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}

void Player::Update()
{
	//�X�e�[�g�}�V�[���̍X�V�B
	UpdateStateMachine();
	//�_���[�W�����B
	Damage();
	//�|�C���g���C�g�̈ʒu���X�V�B
	UpdatePointLightPosition();
	//�A�j���[�V�����R���g���[���B
	AnimationControl();
	//�o�g���p�̃V�[�g�̍X�V�B
	UpdateBattleSeats();
	//�A�j���[�V�����C�x���g�R���g���[���̎��s�B
	animationEventController.Update();
	//�}�W�b�N�|�C���g�͏����Â񕜂���B
	RecoverMagicPoint(RECOVER_MP * GameTime().GetFrameDeltaTime());

	ShadowMap().SetLightTarget(position);
	CVector3 lightPos;
	lightPos.Add(position, toLightPos);
	ShadowMap().SetLightPosition(lightPos);

	skinModel.Update(position, rotation, CVector3::One);


	lastFrameState = state;
}