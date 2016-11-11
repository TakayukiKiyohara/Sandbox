/*!
 * @brief	�G�̏�Ԃ̊��N���X�B
 */

#pragma once

#include "Enemy/Enemy.h"
#include "DamageCollisionWorld.h"

/*!
 * @brief	�G�̏�Ԃ̊��N���X�B
 */
class IEnemyState{
public:
	//��Ԃ�ύX����Ƃ��̈����B
	struct SEnterArg {
		int arg[1];
	};
	IEnemyState( Enemy* enemy ) :
		enemy(enemy),
		m_localState(-1)
	{
		memset(&m_dummy, 0, sizeof(m_dummy));
	}
	virtual ~IEnemyState()
	{
	}

	virtual void Update() = 0;
	virtual void Enter(const SEnterArg& enterArg)
	{
		m_localState = -1;
	}
	virtual void Leave()
	{

	}
	void ChangeLocalState(int nextState)
	{
		ChangeLocalState(nextState, m_dummy);
	}
	void ChangeLocalState( int nextState, const SEnterArg& arg)
	{
		if(m_localState != -1){
			m_localStates[m_localState]->Leave();
		}
		m_localState = nextState;
		m_localStates[m_localState]->Enter(arg);
	}
	//�_���[�W���󂯂邱�Ƃ��o���邩����B
	virtual bool IsPossibleApplyDamage(const DamageCollisionWorld::Collision* colli) const
	{
		return true;
	}
protected:
	std::vector<IEnemyState*>	m_localStates;		//���[�J���B
	int							m_localState;		//���[�J���X�e�[�g�B
	Enemy*						enemy;
	SEnterArg					m_dummy;
};