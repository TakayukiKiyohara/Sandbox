/*!
 * @brief	�����ԁB
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	�����ԁB
 */
class EnemyStateRun : public IEnemyState{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	EnemyStateRun(Enemy* enemy);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~EnemyStateRun();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
};