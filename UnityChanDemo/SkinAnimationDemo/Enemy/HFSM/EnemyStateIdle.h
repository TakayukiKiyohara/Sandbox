/*!
 * @brief	�ҋ@��ԁB
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	������ԁB
 */
class EnemyStateIdle : public IEnemyState{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	EnemyStateIdle(Enemy* enemy);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~EnemyStateIdle();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
};