/*!
 * @brief	ヤラレ状態。
 */
#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateDeath.h"
#include "ParticleParam.h"
#include "Enemy/EnemyParameter.h"
#include "tkEngine/Sound/tkSoundSource.h"

/*!
 * @brief	コンストラクタ。
 */
EnemyStateDeath::EnemyStateDeath( Enemy* enemy ) :
	IEnemyState(enemy),
	timer(0.0f)
{
}
/*!
 * @brief	デストラクタ。
 */
EnemyStateDeath::~EnemyStateDeath()
{
}
void EnemyStateDeath::Update() 
{
	enemy->PlayAnimation(Enemy::enAnimDeath);
	enemy->SetMoveSpeed(0.0f);
	timer += GameTime().GetFrameDeltaTime();
	if(timer > 1.0f){
		for (auto& p : particleEmitterList) {
			DeleteGO(p);
		}
		particleEmitterList.clear();
	}
}
void EnemyStateDeath::Enter(const SEnterArg& enterArg)
{
	IEnemyState::Enter(enterArg);
	const EnemyParam* ep = enemy->GetEnemyParam();
	CMatrix* m = enemy->FindBoneWorldMatrix(ep->bloodEffectBoneName);
	
	if (m != NULL) {
		CVector3 emitPos = ep->bloodEffectOffsetPosition;
		m->Mul(emitPos);
		//パーティクルエミッターを登録。
		for (SParicleEmitParameter& param : bloodEmitterParam) {
			CParticleEmitter* particleEmitter = NewGO<CParticleEmitter>(1);
			particleEmitter->Init(g_random, g_camera->GetCamera(), param, emitPos);
			particleEmitterList.push_back(particleEmitter);
		}
	}
	timer = 0.0f;
	enemy->NotifyDead();
	CSoundSource* se = NewGO<CSoundSource>(0);
	se->Init("Assets/sound/Damage_00.wav");
	se->Play(false);
	se->SetVolume(1.0f);
}
void EnemyStateDeath::Leave()
{
	for (auto& p : particleEmitterList) {
		DeleteGO(p);
	}
	particleEmitterList.clear();
}
