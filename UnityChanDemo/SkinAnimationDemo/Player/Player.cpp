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
	const float USE_MP_DASH = 50.0f;		//ダッシュ魔法を使った時のMP使用量。
	const float RECOVER_MP = 20.0f;			//MP自然回復量。

	const float RUN_THREADHOLD_SQ = 4.0f * 4.0f;		//走りアニメーションを再生する速度の閾値。
	//アニメーションのイベントテーブル。
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
* @brief	開始
*/
void Player::Start()
{
	SkinModelDataResources().Load(skinModelData, "Assets/modelData/Player.X", &animation);
	normalMap.Load("Assets/modelData/Thethief_N.tga");
	specMap.Load("Assets/modelData/Thethief_S.tga");
	weaponNormalMap.Load("Assets/modelData/Thethief_wuqi_N.tga");
	weaponSpecMap.Load("Assets/modelData/Thethief_wuqi_S.tga");
	//体のマテリアルを取得。
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Thethief_D.tga");
	if (mat != NULL) {
		mat->SetTexture("g_normalTexture", &normalMap);
		mat->SetTexture("g_speculerMap", &specMap);
	}
	//武器のマテリアルを取得。
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

	//アニメーションイベントコントローラの初期化。
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
			//Aボタンが押された。
			//車との距離を調べる。
			CVector3 diff = g_car->GetPosition();
			diff.Subtract(position);
			if (!characterController.IsJump()) {
				//ジャンプ。
				moveSpeed.y = 8.0f;
				characterController.Jump();
			}
		}
		//走りか立ち状態の時。
		CVector3 moveDirLocal;
		moveDirLocal.y = 0.0f;
		moveDirLocal.x = Pad(0).GetLStickXF();
		moveDirLocal.z = Pad(0).GetLStickYF();
		const CMatrix& mViewInv = g_camera->GetCamera().GetViewMatrixInv();
		//カメラ空間から見た奥方向のベクトルを取得。
		CVector3 cameraZ;
		cameraZ.x = mViewInv.m[2][0];
		cameraZ.y = 0.0f;		//Y軸いらない。
		cameraZ.z = mViewInv.m[2][2];
		cameraZ.Normalize();	//Y軸を打ち消しているので正規化する。
								//カメラから見た横方向のベクトルを取得。
		CVector3 cameraX;
		cameraX.x = mViewInv.m[0][0];
		cameraX.y = 0.0f;		//Y軸はいらない。
		cameraX.z = mViewInv.m[0][2];
		cameraX.Normalize();	//Y軸を打ち消しているので正規化する。

		CVector3 moveDir;
		moveDir.x = cameraX.x * moveDirLocal.x + cameraZ.x * moveDirLocal.z;
		moveDir.y = 0.0f;	//Y軸はいらない。
		moveDir.z = cameraX.z * moveDirLocal.x + cameraZ.z * moveDirLocal.z;

		float fMoveSpeed = MOVE_SPEED;
		bool isDash = false;
		if (Pad(0).IsPress(enButtonRB2)) {
			//MPの残量を見てダッシュできるか調べる。
			float useMp = USE_MP_DASH * GameTime().GetFrameDeltaTime();
			if (mp - useMp >= 0.0f) {
				//MP使える。
				isDash = true;
			}
			UseMagicPoint(useMp);
		}

		if (isDash) {
			//ダッシュ中可能か調べる。
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
			//走り状態に遷移。
			ChangeState(enStateRun);
		}
		else {
			//立ち状態。
			ChangeState(enStateStand);
		}
		bool isOnGround = characterController.IsOnGround();
		characterController.SetMoveSpeed(moveSpeed);
		characterController.Execute();
		if (isOnGround == false
			&& characterController.IsOnGround()
			) {
			//着地した。
			//着地音を再生。
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
		//移動がピタって止まると気持ちわるいので
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
			//コンボ発生。
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
			//エミッターを削除
			for (auto& p : particleEmitterList) {
				DeleteGO(p);
			}
			particleEmitterList.clear();
		}
	}
	position = characterController.GetPosition();
}

/*!
* @brief	ヤラレ処理。
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
		//ダメージを受けた。
		hp -= dmgColli->damage;
		if (hp <= 0.0f) {
			//死亡
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
* @brief	ポイントライトの位置を更新。
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
* @brief	アニメーション再生。
*/
void Player::PlayAnimation(AnimationNo animNo, float interpolateTime)
{
	if (animation.GetPlayAnimNo() != animNo && animNo != AnimationInvalid) {
		//別のアニメーション
		animation.PlayAnimation(animNo, interpolateTime);
	}
}
/*!
* @brief	アニメーションコントロール。
*/
void Player::AnimationControl()
{
	if (characterController.IsJump()) {
		PlayAnimation(AnimationJump, 0.05f);
	}else{
		if (state == enStateRun) {
			if (characterController.GetMoveSpeed().LengthSq() > RUN_THREADHOLD_SQ) {
				//走りアニメーションを流す。
				PlayAnimation(AnimationRun, 0.1f);
			}
			else {
				//歩きアニメーション。
				PlayAnimation(AnimationWalk, 0.1f);
			}
		}
		else if (state == enStateStand) {
			//立ちアニメーションを流す。
			PlayAnimation(AnimationStand, 0.3f);
		}
		else if (state == enState_Attack) {
			if (nextAttackAnimNo == AnimationAttack_Start) {
				//攻撃開始。
				PlayAnimation(nextAttackAnimNo, 0.1f);
				reqAttackAnimNo = nextAttackAnimNo;
				nextAttackAnimNo = AnimationInvalid;
			}else if( nextAttackAnimNo != AnimationInvalid ){
				//連撃のアニメーションをリクエストキューに積む。
				animation.PlayAnimationQueue(nextAttackAnimNo, 0.05f);
				reqAttackAnimNo = nextAttackAnimNo;
				nextAttackAnimNo = AnimationInvalid;
			}
		}
		else if (state == enState_Damage) {
			if (isApplyDamageTrigger) {
				//別のアニメーション
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
* @brief	バトルで使用するシートを初期化。
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
* @brief	バトルで使用するシートを更新。
*/
void Player::UpdateBattleSeats()
{
	for (auto& seat : battleSeats) {
		seat.position.Add(seat.localPosition, position);
	}
}
/*!
* @brief	未使用のシートを検索。
*@param[in]	pos		未使用のシートの中からこの座標に一番近いシートを返します。
*@return		未使用のシートがない場合はNULLが返ってくる。
*/
Player::SBattleSeat* Player::FindUnuseSeat(const CVector3& pos)
{
	float dist = FLT_MAX;
	float distTmp;
	SBattleSeat* result = NULL;
	for (auto& seat : battleSeats) {
		if (!seat.isUse) {
			//未使用。
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
* @brief	状態切り替え。
*/
void Player::ChangeState(EnState nextState)
{
	if (state == enState_Damage
		|| state == enState_Dead
	) {
		//現在のステートがダメージ状態の場合。
		//血しぶきのパーティクルエミッターを削除。
		for (auto& p : particleEmitterList) {
			DeleteGO(p);
		}
		particleEmitterList.clear();
	}
	state = nextState;
	if (state == enState_Damage
		|| state == enState_Dead)
	{
		//次のステートがダメージ状態or死亡状態の場合。
		//血しぶきのエフェクトをエミット。
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
* @brief	描画。
*/
void Player::Render(CRenderContext& renderContext)
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}

void Player::Update()
{
	//ステートマシーンの更新。
	UpdateStateMachine();
	//ダメージ処理。
	Damage();
	//ポイントライトの位置を更新。
	UpdatePointLightPosition();
	//アニメーションコントロール。
	AnimationControl();
	//バトル用のシートの更新。
	UpdateBattleSeats();
	//アニメーションイベントコントローラの実行。
	animationEventController.Update();
	//マジックポイントは少しづつ回復する。
	RecoverMagicPoint(RECOVER_MP * GameTime().GetFrameDeltaTime());

	ShadowMap().SetLightTarget(position);
	CVector3 lightPos;
	lightPos.Add(position, toLightPos);
	ShadowMap().SetLightPosition(lightPos);

	skinModel.Update(position, rotation, CVector3::One);


	lastFrameState = state;
}