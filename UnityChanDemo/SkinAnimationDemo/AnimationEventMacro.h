/*!
 * @brief	�A�j���[�V�����C�x���g�֌W�̃}�N���B
 */

#pragma once

/*!
 * @brief	�A�j���[�V�����C�x���g�̏I�[�B
 */
#define END_ANIMATION_EVENT()													\
	AnimationEvent(																\
		eAnimationEventType_Invalid,											\
		0.0f,																	\
		0.0f, 0.0f, 0.0f, 0.0f, 												\
		0, 0, 0, 0, 															\
		NULL, NULL, NULL, NULL, 												\
		CVector3::Zero, CVector3::Zero, CVector3::Zero, CVector3::Zero	 		\
	)

/*!
 * @brief	�G�Ƀ_���[�W��^����	�R���W���������C�x���g�B
 *@param[in]	invokeTime			�R���W�������������鎞�ԁB�Y������A�j���[�V�������Đ�����Ă���o�ߎ��Ԃł��B�P�ʂ͕b�B
 *@param[in]	life				�R���W�������������Ă��鎞�ԁB�P�ʂ͕b�B
 *@param[in]	radius				�R���W�����̔��a�B
 *@param[in]	damage				�_���[�W�ʁB
 *@param[in]	boneName			�{�[�����B
 *@param[in]	offsetLocalVector	�{�[������̃I�t�Z�b�g���W�B
 *@param[in]	groupID				�O���[�v�h�c�B
 */
#define EMIT_DAMAGE_TO_ENEMY_COLLISION_EVENT(invokeTime, life, radius, damage, boneName, offsetLocalVector, groupID)		\
	AnimationEvent(																				\
		eAnimationEventType_EmitDamageToEnemyCollision,											\
		(invokeTime),																			\
		(life), (radius), 0.0f, 0.0f,															\
		damage,groupID,0,0,																		\
		boneName, NULL, NULL, NULL,																\
		offsetLocalVector, CVector3::Zero, CVector3::Zero, CVector3::Zero						\
	)

 /*!
 * @brief	�v���C���Ƀ_���[�W��^����	�R���W���������C�x���g�B
 *@param[in]	invokeTime			�R���W�������������鎞�ԁB�Y������A�j���[�V�������Đ�����Ă���o�ߎ��Ԃł��B�P�ʂ͕b�B
 *@param[in]	life				�R���W�������������Ă��鎞�ԁB�P�ʂ͕b�B
 *@param[in]	radius				�R���W�����̔��a�B
 *@param[in]	damage				�_���[�W�ʁB
 *@param[in]	boneName			�{�[�����B
 *@param[in]	offsetLocalVector	�{�[������̃I�t�Z�b�g���W�B
 *@param[in]	groupID				�O���[�v�h�c�B
 */
#define EMIT_DAMAGE_TO_PLAYER_COLLISION_EVENT(invokeTime, life, radius, damage, boneName, offsetLocalVector, groupID)		\
	AnimationEvent(																				\
		eAnimationEventType_EmitDamageToPlayerCollision,											\
		(invokeTime),																			\
		(life), (radius), 0.0f, 0.0f,															\
		damage,groupID,0,0,																		\
		boneName, NULL, NULL, NULL,																\
		offsetLocalVector, CVector3::Zero, CVector3::Zero, CVector3::Zero						\
	)
 /*!
 * @brief	�T�E���h�̔����C�x���g
 *@param[in]	invokeTime			�T�E���h�𔭐������鎞�ԁB�Y������A�j���[�V�������Đ�����Ă���o�ߎ��Ԃł��B�P�ʂ͕b�B
 *@param[in]	volume				�{�����[��
 *@param[in]	filePath			�T�E���h�̃t�@�C���p�X�B
 */
#define EMIT_SOUND_EVENT(invokeTime, volume, filePath)											\
	AnimationEvent(																				\
		eAnimationEventType_EmitSound,															\
		(invokeTime),																			\
		(volume), 0.0f, 0.0f, 0.0f,																\
		0,0,0,0,																				\
		filePath, NULL, NULL, NULL,																\
		CVector3::Zero, CVector3::Zero, CVector3::Zero, CVector3::Zero							\
	)

 /*!
 * @brief	3D�T�E���h�̔����C�x���g
 *@param[in]	invokeTime			�T�E���h�𔭐������鎞�ԁB�Y������A�j���[�V�������Đ�����Ă���o�ߎ��Ԃł��B�P�ʂ͕b�B
 *@param[in]	volume				�{�����[��
 *@param[in]	filePath			�T�E���h�̃t�@�C���p�X�B
 */
#define EMIT_3D_SOUND_EVENT(invokeTime, volume, filePath)										\
	AnimationEvent(																				\
		eAnimationEventType_Emit3DSound,														\
		(invokeTime),																			\
		(volume), 0.0f, 0.0f, 0.0f,																\
		0,0,0,0,																				\
		filePath, NULL, NULL, NULL,																\
		CVector3::Zero, CVector3::Zero, CVector3::Zero, CVector3::Zero							\
	)

