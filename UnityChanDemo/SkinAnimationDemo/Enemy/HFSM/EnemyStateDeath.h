/*!
 * @brief	死亡状態。
 */

#pragma once 

#include "IEnemyState.h"

namespace tkEngine {
	class CParticleEmitter;
}

class EnemyStateDeath : public IEnemyState{
private:
	std::list<CParticleEmitter*>	particleEmitterList;
	float timer;		//タイマ。
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateDeath( Enemy* enemy );
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateDeath();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
	//ダメージを受けることが出来るか判定。
	bool IsPossibleApplyDamage(const DamageCollisionWorld::Collision* colli) const override
	{
		return false;
	}
};