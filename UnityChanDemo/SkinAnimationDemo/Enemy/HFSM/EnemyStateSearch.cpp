/*!
 * @brief	索敵状態。
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateSearch.h"
#include "Enemy/HFSM/EnemyStateWalk.h"
#include "Enemy/HFSM/EnemyStateIdle.h"


/*!
 * @brief	コンストラクタ。
 */
EnemyStateSearch::EnemyStateSearch( Enemy* enemy ) :
	IEnemyState(enemy)
{
	m_localStates.push_back( new EnemyStateWalk(enemy) );
	m_localStates.push_back( new EnemyStateIdle(enemy) );
}
/*!
 * @brief	デストラクタ。
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
			//1秒間に50%の確率で待機状態に遷移する。。
			if (g_random.GetRandDouble() < 0.5f) {
				ChangeLocalState(enLocalState_Idle);
			}
			timer = 0.0f;
		}
	}else{
		timer += GameTime().GetFrameDeltaTime();
		if (timer > 2.0f) {
			//1秒間に50%の確率で方向転換。
			if (g_random.GetRandDouble() < 0.5f) {
				float t = (float)g_random.GetRandDouble();
				//最大180度回転
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
