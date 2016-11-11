/*!
 * @brief	発見状態。
 */

#pragma once

#include "Player/Player.h"
#include "IEnemyState.h"

/*!
 * @brief	索敵状態。
 */
class EnemyStateFind : public IEnemyState{
private:

	enum EnLocalState{
		enLocalState_Run,
		enLocalState_Attack,
		enLocalState_Idle,
	};
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	EnemyStateFind(Enemy* enemy);
	/*!
	 * @brief	デストラクタ。
	 */
	~EnemyStateFind();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
private:
	float timer;
	Player::SBattleSeat*		battleSeat;
};