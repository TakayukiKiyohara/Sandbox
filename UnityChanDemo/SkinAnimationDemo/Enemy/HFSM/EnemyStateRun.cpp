/*!
 * @brief	�����ԁB
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateRun.h"


/*!
 * @brief	�R���X�g���N�^�B
 */
EnemyStateRun::EnemyStateRun( Enemy* enemy ) :
	IEnemyState(enemy)
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
EnemyStateRun::~EnemyStateRun()
{
}
void EnemyStateRun::Update()
{
	enemy->PlayAnimation(Enemy::enAnimWalk);
	enemy->SetMoveSpeed(3.0f);
}
void EnemyStateRun::Enter(const SEnterArg& enterArg)
{
	IEnemyState::Enter(enterArg);
}
void EnemyStateRun::Leave() 
{
}
