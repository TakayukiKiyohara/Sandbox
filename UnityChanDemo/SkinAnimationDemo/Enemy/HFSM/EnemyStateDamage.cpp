/*!
 * @brief	ヤラレ状態。
 */
#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateDamage.h"
#include "tkEngine/particle/tkParticleEmitter.h"
#include "GameCamera.h"
#include "Player/Player.h"
#include "ParticleParam.h"
#include "Enemy/EnemyParameter.h"
#include "tkEngine/Sound/tkSoundSource.h"

/*!
 * @brief	コンストラクタ。
 */
EnemyStateDamage::EnemyStateDamage(Enemy* enemy) :
	IEnemyState(enemy),
	dmgCollisionGroupID(0)
{
}
/*!
 * @brief	デストラクタ。
 */
EnemyStateDamage::~EnemyStateDamage()
{
}
void EnemyStateDamage::Update() 
{
	//enemy->PlayAnimation(Enemy::enAnimDamage);
	enemy->SetMoveSpeed(0.0f);
}
void EnemyStateDamage::Enter(const SEnterArg& enterArg)
{
	IEnemyState::Enter(enterArg);
	dmgCollisionGroupID = enterArg.arg[0];
	enemy->PlayAnimationForce(Enemy::enAnimDamage);
	
	const EnemyParam* enemyParam = enemy->GetEnemyParam();
	CMatrix* m = enemy->FindBoneWorldMatrix(enemyParam->bloodEffectBoneName);
	if (m != NULL) {
		//パーティクルエミッターを登録。
		for (SParicleEmitParameter& param : bloodEmitterParam) {
			CParticleEmitter* particleEmitter = NewGO<CParticleEmitter>(1);
			CVector3 pos;
			pos.Set(m->m[3][0], m->m[3][1], m->m[3][2]);
			particleEmitter->Init(g_random, g_camera->GetCamera(), param, pos);
			particleEmitterList.push_back(particleEmitter);
		}
	}
	
	CSoundSource* se = NewGO<CSoundSource>(0);
	se->Init("Assets/sound/Damage_00.wav");
	se->Play(false);
	se->SetVolume(1.0f);
}
void EnemyStateDamage::Leave()
{
	for(auto& p : particleEmitterList){
		DeleteGO(p);
	}
	particleEmitterList.clear();
}
