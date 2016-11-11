/*!
 * @brief	���G��ԁB
 */

#pragma once

#include "IEnemyState.h"

/*!
 * @brief	���G��ԁB
 */
class EnemyStateSearch : public IEnemyState{
private:
	enum EnLocalState{
		enLocalState_Walk,
		enLocalState_Idle,
	};
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	EnemyStateSearch(Enemy* enemy);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~EnemyStateSearch();
	void Update() override;
	void Enter(const SEnterArg& enterArg) override;
	void Leave() override;
private:
	float			timer;				//�^�C�}
};