/*!
 * @brief	�A�j���[�V�����C�x���g�R���g���[���B
 */

#pragma once

#include "AnimationEventMacro.h"

const int ANIMATION_EVENT_MAX = 32;

/*!
 * @brief	�A�j���[�V�����C�x���g�̎�ށB
 */
enum EAnimationEventType{
	eAnimationEventType_EmitDamageToEnemyCollision,		//�G�ɑ΂��ă_���[�W��^����R���W�����𐶐��B
	eAnimationEventType_EmitDamageToPlayerCollision,	//�v���C���ɑ΂��ă_���[�W��^����R���W�����𐶐��B
	eAnimationEventType_EmitSound,						//Sound�̍Đ��B
	eAnimationEventType_Emit3DSound,					//3DSound�̍Đ��B
	eAnimationEventType_Invalid = 0xffffffff,
};
/*!
 * @brief	�A�j���[�V�����C�x���g�B
 */
struct AnimationEvent{
	AnimationEvent(){}
	AnimationEvent(
		EAnimationEventType type,
		float time,
		float fArg0, float fArg1, float fArg2, float fArg3,
		int iArg0, int iArg1, int iArg2, int iArg3,
		const char* strArg0, const char* strArg1, const char* strArg2, const char* strArg3,
		CVector3 vArg0, CVector3 vArg1, CVector3 vArg2, CVector3 vArg3
		)
	{
		this->eventType = type;
		this->time = time;
		fArg[0] = fArg0;
		fArg[1] = fArg1;
		fArg[2] = fArg2;
		fArg[3] = fArg3;

		iArg[0] = iArg0;
		iArg[1] = iArg1;
		iArg[2] = iArg2;
		iArg[3] = iArg3;

		strArg[0] = strArg0;
		strArg[1] = strArg1;
		strArg[2] = strArg2;
		strArg[3] = strArg3;

		vArg[0] = vArg0;
		vArg[1] = vArg1;
		vArg[2] = vArg2;
		vArg[3] = vArg3;
	}
	EAnimationEventType eventType;			//�C�x���g�̃^�C�v�B
	float 				time;				//�C�x���g���������鎞�ԁB�Y������A�j���[�V�������Đ�����Čo�߂������Ԃł��B�P�ʂ͕b�B
	float				fArg[4];			//���������_�^�̈����B
	int					iArg[4];			//�����^�̈����B
	const char*			strArg[4];			//����������B
	CVector3			vArg[4];			//�x�N�g���^�̈����B
};
/*!
 * @brief	�A�j���[�V�����O���[�v�B�ЂƂ̃A�j���[�V�����Ŕ�������C�x���g���O���[�v���������́B
 */
struct AnimationEventGroup{
	AnimationEvent event[ANIMATION_EVENT_MAX];
};

/*!
 * @brief	�A�j���[�V�����C�x���g�R���g���[���B
 */
class AnimationEventController{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	AnimationEventController();
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~AnimationEventController();
	/*!
	 * @brief	�������B
	 *@code
	 
	 *@endcode
	 *@param[in]	skinModel		�X�L�����f���B
	 *@param[in]	animation		�A�j���[�V�����B
	 *@param[in]	eventGroupTbl	�C�x���g�O���[�v�̃e�[�u���B
	 *@param[in]	tblSize			�C�x���g�O���[�v�̃e�[�u���̃T�C�Y�B
	 */
	void Init(CSkinModel* skinModel, CAnimation* animation, AnimationEventGroup* eventGroupTbl, int tblSize );
	/*!
	 * @brief	�X�V�B
	 */
	void Update();
private:
	/*!
	 * @brief	�A�j���[�V�����C�x���g�̔����B
	 */
	void InvokeAnimationEvent(const AnimationEvent& event);
private:
	struct AnimationEventGroupEx{
		AnimationEventGroup eventGroup;
		bool invokeFlags[ANIMATION_EVENT_MAX];	//�C�x���g�̔����t���O�B
	};
	int									animNoLastFrame = -1;		//!<��t���[���O�̃A�j���[�V�����ԍ��B
	CAnimation* 						animation = nullptr;		//!<�A�j���[�V�����B
	std::vector<AnimationEventGroupEx>	eventGroupExTbl;	
	CSkinModel*							skinModel = nullptr;		//!<�X�L�����f���B
	float								lastFrameAnimTime = 0.0f;	//!<1�t���[���O�̃A�j���[�V�����^�C���B
	bool								isInited = false;			//!<�������ł������ǂ����̃t���O�B
};