/*!
 *@brief	�G�l�~�[�̃Z���T�[�B
 */

#pragma once

class Enemy;
/*!
 *@brief	�G�l�~�[�̃Z���T�[�N���X�B
 * ���̏������W���邽�߂̃N���X�B
 */
class EnemySensor{
public:
	EnemySensor();
	~EnemySensor();
	void Update();
private:
	Enemy*	enemy;		//�G�l�~�[�B
};