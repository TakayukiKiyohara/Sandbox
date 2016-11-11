/*!
 * @brief	��������ԁB
 */

#pragma once 

#include "IEnemyState.h"


namespace tkEngine {
	class CParticleEmitter;
}
class EnemyStateDamage : public IEnemyState{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	EnemyStateDamage( Enemy* enemy );
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~EnemyStateDamage();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
	bool IsPossibleApplyDamage(const DamageCollisionWorld::Collision* colli) const override
	{
		return dmgCollisionGroupID != colli->groupID;	//�����O���[�v�h�c�̃R���W��������̓_���[�W���󂯂Ȃ��B
	}
private:
	int dmgCollisionGroupID;	//�_���[�W���󂯂��R���W�����̃O���[�v�h�c�B
	std::list<CParticleEmitter*>	particleEmitterList;
};