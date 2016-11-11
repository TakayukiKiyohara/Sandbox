/*!
 * @brief	敵のアニメーションイベントのテーブル。
 */

#pragma once

#include "AnimationEventController.h"
#include "Enemy/Enemy.h"

//Enemy00のアニメーションイベントテーブル。
static AnimationEventGroup cEnemy00_AnimationEventGroupTbl[Enemy::enNumAnim] = {
	//enAnimStand
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimWalk
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimAttack
	{
		EMIT_DAMAGE_TO_PLAYER_COLLISION_EVENT(1.0f, 0.3f, 0.7f, 10, "Bip001_R_Hand", CVector3(0.0f, 0.0f, 0.0f), 0),
		EMIT_3D_SOUND_EVENT( 0.5f, 1.0f, "Assets/sound/EnemyAttack.wav"),
		END_ANIMATION_EVENT(),
	},
	//enAnimDamage
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimDeath
	{
		END_ANIMATION_EVENT(),
	},
};


//Enemy01のアニメーションイベントテーブル。
static AnimationEventGroup cEnemy01_AnimationEventGroupTbl[Enemy::enNumAnim] = {
	//enAnimStand
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimWalk
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimAttack
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimDamage
	{
		END_ANIMATION_EVENT(),
	},
	//enAnimDeath
	{
		END_ANIMATION_EVENT(),
	},
};