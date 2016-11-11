/*!
 * @brief	������ԁB
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	������ԁB
 */
class EnemyStateWalk : public IEnemyState{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	EnemyStateWalk(Enemy* enemy);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~EnemyStateWalk();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
};