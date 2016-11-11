/*!
 *@brief	�G�̃p�����[�^�e�[�u���B
 */

#pragma once

#include "AnimationEventController.h"
#include "Enemy/EnemyAnimationEventTbl.h"
/*!
 *@brief	�G�̃p�����[�^�B
 */
struct EnemyParam{
	const char* name;					//���O�B
	const char* bloodEffectBoneName;	//�����Ԃ��G�t�F�N�g��\������{�[���̖��O�B
	CVector3 bloodEffectOffsetPosition;	//�����Ԃ��G�t�F�N�g��\������I�t�Z�b�g���W�B
	AnimationEventGroup* animationEventGroup;	//�A�j���[�V�����C�x���g�O���[�v�B
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
	//�f�t�H���g�B
	{
		"default",
		""
	},
};