/*!
 * @brief	走り状態。
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateRun.h"


/*!
 * @brief	コンストラクタ。
 */
EnemyStateRun::EnemyStateRun( Enemy* enemy ) :
	IEnemyState(enemy)
{
}
/*!
 * @brief	デストラクタ。
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
