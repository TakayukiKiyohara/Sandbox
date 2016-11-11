/*!
 * @brief	������ԁB
 */

#include "stdafx.h"
#include "Enemy/HFSM/EnemyStateFind.h"
#include "Enemy/HFSM/EnemyStateRun.h"
#include "Enemy/HFSM/EnemyStateAttack.h"
#include "Enemy/HFSM/EnemyStateIdle.h"
#include "Player/Player.h"

namespace {
	const float ENEMY_ATTACK_READY_RANGE_SQ = 1.5f * 1.5f;
	const float ENEMY_ATTACK_RANGE_SQ = 0.3f * 0.3f;
}
/*!
 * @brief	�R���X�g���N�^�B
 */
EnemyStateFind::EnemyStateFind(Enemy* enemy) :
	IEnemyState(enemy),
	timer(0.0f),
	battleSeat(nullptr)
{
	m_localStates.push_back( new EnemyStateRun(enemy) );
	m_localStates.push_back( new EnemyStateAttack(enemy) );
	m_localStates.push_back(new EnemyStateIdle(enemy));
}
/*!
 * @brief	�f�X�g���N�^�B
 */
EnemyStateFind::~EnemyStateFind()
{
}
void EnemyStateFind::Update()
{
	m_localStates[m_localState]->Update();
	CVector3 toPlayer;
	toPlayer.Subtract(g_player->GetPosition(), enemy->GetPosition());
	toPlayer.y = 0.0f;
	if (m_localState == enLocalState_Run) {
		CVector3 moveDirTmp;
		//�A�^�b�NReady
		//�V�[�g��T���B
		battleSeat = g_player->FindUnuseSeat(enemy->GetPosition());
		if (battleSeat != nullptr) {
			//�V�[�g�����������B
			battleSeat->isUse = true;
		}
		if (battleSeat == nullptr) {
			//�ҋ@��ԂɁB
			ChangeLocalState(enLocalState_Idle);
		}
		
		if (battleSeat != nullptr) {
			//�o�g���V�[�g�����������B�U�����������B
			moveDirTmp.Subtract(battleSeat->position, enemy->GetPosition());
			moveDirTmp.y = 0.0f;
			if (moveDirTmp.LengthSq() < ENEMY_ATTACK_RANGE_SQ) {
				//�U�������W�ɓ������B
				ChangeLocalState(enLocalState_Attack);
			}
			else {
				battleSeat->isUse = false;
				battleSeat = nullptr;
			}
		}
		if (moveDirTmp.LengthSq() > 0.01f) {
			moveDirTmp.Normalize();
			enemy->SetMoveDirection(moveDirTmp);
		}
	}else if(m_localState == enLocalState_Attack){
		CVector3 dist;
		dist.Subtract(battleSeat->position, enemy->GetPosition());
		dist.y = 0.0f;
		if (!enemy->IsPlayAnimation()) {
			ChangeLocalState(enLocalState_Idle);
		}
		if (dist.LengthSq() > ENEMY_ATTACK_RANGE_SQ && !enemy->IsPlayAnimation()) {
			ChangeLocalState(enLocalState_Run);
			battleSeat->isUse = false;
			battleSeat = nullptr;
		}
	}
	else if (m_localState == enLocalState_Idle) {
		timer += GameTime().GetFrameDeltaTime();
		if (timer > 0.2f) {
			CVector3 dist;
			if (battleSeat != nullptr) {
				//�o�g���V�[�g�������Ă���B
				dist.Subtract(battleSeat->position, enemy->GetPosition());
				dist.y = 0.0f;
				if (dist.LengthSq() > ENEMY_ATTACK_RANGE_SQ) {
					ChangeLocalState(enLocalState_Run);
					battleSeat->isUse = false;
					battleSeat = nullptr;
				}
				else {
					//�U���B
					ChangeLocalState(enLocalState_Attack);
				}
			}
			else {
				//�o�g���V�[�g�������Ă��Ȃ��B
				ChangeLocalState(enLocalState_Run);
			}
			timer = 0.0f;
		}
	}
	if (toPlayer.LengthSq() > 0.01f) {
		toPlayer.Normalize();
		enemy->SetDirection(toPlayer);
	}
}
void EnemyStateFind::Enter(const SEnterArg& enterArg)
{
	IEnemyState::Enter(enterArg);
	timer = 0.0f;
	battleSeat = NULL;
	ChangeLocalState(enLocalState_Run);
}
void EnemyStateFind::Leave() 
{
	IEnemyState::Leave();
	if (battleSeat) {
		battleSeat->isUse = false;
	}
}
