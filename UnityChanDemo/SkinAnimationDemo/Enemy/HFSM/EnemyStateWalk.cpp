/*!
 * @brief	歩き状態。
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateWalk.h"


/*!
 * @brief	コンストラクタ。
 */
EnemyStateWalk::EnemyStateWalk( Enemy* enemy ) :
	IEnemyState(enemy)
{
}
/*!
 * @brief	デストラクタ。
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
