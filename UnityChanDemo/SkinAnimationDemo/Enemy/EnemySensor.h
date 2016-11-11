/*!
 *@brief	エネミーのセンサー。
 */

#pragma once

class Enemy;
/*!
 *@brief	エネミーのセンサークラス。
 * 環境の情報を収集するためのクラス。
 */
class EnemySensor{
public:
	EnemySensor();
	~EnemySensor();
	void Update();
private:
	Enemy*	enemy;		//エネミー。
};