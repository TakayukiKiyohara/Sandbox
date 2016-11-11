/*!
 *@brief	敵のパラメータテーブル。
 */

#pragma once

#include "AnimationEventController.h"
#include "Enemy/EnemyAnimationEventTbl.h"
/*!
 *@brief	敵のパラメータ。
 */
struct EnemyParam{
	const char* name;					//名前。
	const char* bloodEffectBoneName;	//血しぶきエフェクトを表示するボーンの名前。
	CVector3 bloodEffectOffsetPosition;	//血しぶきエフェクトを表示するオフセット座標。
	AnimationEventGroup* animationEventGroup;	//アニメーションイベントグループ。
};
static const EnemyParam g_enemyParamTable[] = {
	//enemy00
	{
		"enemy_00",
		"Bip001_Neck",
		CVector3::Zero,
		cEnemy00_AnimationEventGroupTbl
	},
	//enemy01
	{
		"enemy_01",
		"spine",
		CVector3::Zero,
		cEnemy01_AnimationEventGroupTbl
	},
	//デフォルト。
	{
		"default",
		""
	},
};