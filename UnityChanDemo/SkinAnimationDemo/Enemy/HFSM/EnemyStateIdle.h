/*!
 * @brief	待機状態。
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	歩き状態。
 */
class EnemyStateIdle : public IEnemyState{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateIdle(Enemy* enemy);
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateIdle();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
};