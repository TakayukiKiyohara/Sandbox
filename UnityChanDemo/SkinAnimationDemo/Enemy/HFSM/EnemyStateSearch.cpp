/*!
 * @brief	���G��ԁB
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateSearch.h"
#include "Enemy/HFSM/EnemyStateWalk.h"
#include "Enemy/HFSM/EnemyStateIdle.h"


/*!
 * @brief	�R���X�g���N�^�B
 */
EnemyStateSearch::EnemyStateSearch( Enemy* enemy ) :
	IEnemyState(enemy)
{
	m_localStates.push_back( new EnemyStateWalk(enemy) );
	m_localStates.push_back( new EnemyStateIdle(enemy) );
}
/*!
 * @brief	�f�X�g���N�^�B
 */
EnemyStateSearch::~EnemyStateSearch()
{
}
void EnemyStateSearch::Update()
{
	m_localStates[m_localState]->Update();
	if(m_localState == enLocalState_Walk){
		timer += GameTime().GetFrameDeltaTime();
		if (timer > 2.0f) {
			//1�b�Ԃ�50%�̊m���őҋ@��ԂɑJ�ڂ���B�B
			if (g_random.GetRandDouble() < 0.5f) {
				ChangeLocalState(enLocalState_Idle);
			}
			timer = 0.0f;
		}
	}else{
		timer += GameTime().GetFrameDeltaTime();
		if (timer > 2.0f) {
			//1�b�Ԃ�50%�̊m���ŕ����]���B
			if (g_random.GetRandDouble() < 0.5f) {
				float t = (float)g_random.GetRandDouble();
				//�ő�180�x��]
				float turnAngle = t * CMath::PI;
				CMatrix mRot;
				mRot.MakeRotationY(turnAngle);
				CVector3 dir = enemy->GetMoveDirection();
				mRot.Mul(dir);
				enemy->SetMoveDirection(dir);
				enemy->SetDirection(dir);
				ChangeLocalState(enLocalState_Walk);
			}
			timer = 0.0f;
		}
	}
	
}
void EnemyStateSearch::Enter(const SEnterArg& enterArg)
{
	IEnemyState::Enter(enterArg);
	timer = 0.0f;
	ChangeLocalState(enLocalState_Walk);
}
void EnemyStateSearch::Leave() 
{
	IEnemyState::Leave();
	m_localStates[m_localState]->Leave();
}
