/*!
 * @brief	索敵状態。
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	索敵状態。
 */
class EnemyStateSearch : public IEnemyState{
private:
	enum EnLocalState{
		enLocalState_Walk,
		enLocalState_Idle,
	};
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateSearch(Enemy* enemy);
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateSearch();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
private:
	float			timer;				//タイマ
};