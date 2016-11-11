/*!
 * @brief	歩き状態。
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	歩き状態。
 */
class EnemyStateWalk : public IEnemyState{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateWalk(Enemy* enemy);
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateWalk();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
};