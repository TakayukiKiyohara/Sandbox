/*!
 * @brief	���S��ԁB
 */

#pragma once 

#include "IEnemyState.h"

namespace tkEngine {
	class CParticleEmitter;
}

class EnemyStateDeath : public IEnemyState{
private:
	std::list<CParticleEmitter*>	particleEmitterList;
	float timer;		//�^�C�}�B
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	EnemyStateDeath( Enemy* enemy );
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~EnemyStateDeath();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
	//�_���[�W���󂯂邱�Ƃ��o���邩����B
	bool IsPossibleApplyDamage(const DamageCollisionWorld::Collision* colli) const override
	{
		return false;
	}
};