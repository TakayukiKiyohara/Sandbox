/*!
 * @brief	走り状態。
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	走り状態。
 */
class EnemyStateRun : public IEnemyState{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateRun(Enemy* enemy);
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateRun();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
};