/*!
 * @brief	������ԁB
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateWalk.h"


/*!
 * @brief	�R���X�g���N�^�B
 */
EnemyStateWalk::EnemyStateWalk( Enemy* enemy ) :
	IEnemyState(enemy)
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
EnemyStateWalk::~EnemyStateWalk()
{
}
void EnemyStateWalk::Update()
{
	enemy->PlayAnimation(Enemy::enAnimWalk);
	enemy->SetMoveSpeed(1.0f);
}
void EnemyStateWalk::Enter(const SEnterArg& enterArg)
{
	IEnemyState::Enter(enterArg);
}
void EnemyStateWalk::Leave() 
{
}
