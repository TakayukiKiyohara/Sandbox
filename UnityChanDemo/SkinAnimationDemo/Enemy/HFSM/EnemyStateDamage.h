/*!
 * @brief	ヤラレ状態。
 */

#pragma once 

#include "IEnemyState.h"


namespace tkEngine {
	class CParticleEmitter;
}
class EnemyStateDamage : public IEnemyState{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateDamage( Enemy* enemy );
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateDamage();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
	bool IsPossibleApplyDamage(const DamageCollisionWorld::Collision* colli) const override
	{
		return dmgCollisionGroupID != colli->groupID;	//同じグループＩＤのコリジョンからはダメージを受けない。
	}
private:
	int dmgCollisionGroupID;	//ダメージを受けたコリジョンのグループＩＤ。
	std::list<CParticleEmitter*>	particleEmitterList;
};