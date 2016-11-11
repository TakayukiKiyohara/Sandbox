/*!
 * @brief	�A�j���[�V�����C�x���g�R���g���[���B
 */

#include "stdafx.h"
#include "AnimationEventController.h"
#include "DamageCollisionWorld.h"
#include "tkEngine/Sound/tkSoundSource.h"

AnimationEventController::AnimationEventController() 
{
}
AnimationEventController::~AnimationEventController()
{
}
void AnimationEventController::Init(CSkinModel* skinModel, CAnimation* animation, AnimationEventGroup* eventGroupTbl, int tblSize )
{
	TK_ASSERT(animation->GetNumAnimationSet() == tblSize, "tblSize is invalid");
	if (eventGroupTbl == NULL) {
		return;
	}
	this->skinModel = skinModel;
	this->animation = animation;
	eventGroupExTbl.resize(tblSize);
	for (int i = 0; i < tblSize; i++) {
		eventGroupExTbl[i].eventGroup = eventGroupTbl[i];
		memset(eventGroupExTbl[i].invokeFlags, 0, sizeof(eventGroupExTbl[i].invokeFlags));
	}
	isInited = true;
}
void AnimationEventController::InvokeAnimationEvent(const AnimationEvent& event)
{
	switch (event.eventType) {
	case eAnimationEventType_EmitDamageToEnemyCollision: {
		//�G�Ƀ_���[�W��^����R���W�����̔����B
		CMatrix* bone = skinModel->FindBoneWorldMatrix(event.strArg[0]);
		CVector3 pos = event.vArg[0];
		if (bone != NULL) {
			bone->Mul(pos);
			g_damageCollisionWorld->Add(event.fArg[1], pos, event.fArg[0], event.iArg[0], DamageCollisionWorld::enDamageToEnemy, event.iArg[1]);
		}
		else {
			TK_LOG("bone is null AnimationEventController::InvokeAnimationEvent");
		}
	}break;
	case eAnimationEventType_EmitDamageToPlayerCollision: {
		//�v���C���[�Ƀ_���[�W��^����R���W�����̔����B
		CMatrix* bone = skinModel->FindBoneWorldMatrix(event.strArg[0]);
		CVector3 pos = event.vArg[0];
		if (bone != NULL) {
			bone->Mul(pos);
			g_damageCollisionWorld->Add(event.fArg[1], pos, event.fArg[0], event.iArg[0], DamageCollisionWorld::enDamageToPlayer, event.iArg[1]);
		}
		else {
			TK_LOG("bone is null AnimationEventController::InvokeAnimationEvent");
		}
	}break;
	case eAnimationEventType_EmitSound: {
		//�����V���b�g�̃T�E���h�����B
		CSoundSource* s = NewGO<CSoundSource>(0);
		s->Init((char*)event.strArg[0]);
		s->Play(false);
		s->SetVolume(event.fArg[0]);
	}
	case eAnimationEventType_Emit3DSound: {
		CSoundSource* s = NewGO<CSoundSource>(0);
		s->Init((char*)event.strArg[0], true);
		const CMatrix& m = skinModel->GetWorldMatrix();
		s->SetPosition({ m.m[3][0], m.m[3][1], m.m[3][2] });
		s->Play(false);
		s->SetVolume(event.fArg[0]);
	}break;
	default:
		break;
	}
}
void AnimationEventController::Update()
{
	if (isInited == false) {
		//���������������Ă��Ȃ��B
		return;
	}
	TK_ASSERT(animation != nullptr, "animation is null");
	int currentAnimNo = animation->GetPlayAnimNo();
	float animTime = animation->GetLocalAnimationTime();
	if (animNoLastFrame != -1 && animNoLastFrame != currentAnimNo) {
		//�A�j���[�V�������؂�ւ�����B
		//�O�̃A�j���[�V�����̃C�x���g�����t���O������������B
		memset(eventGroupExTbl[animNoLastFrame].invokeFlags, 0, sizeof(eventGroupExTbl[animNoLastFrame].invokeFlags));
	}
	else if (animTime < lastFrameAnimTime) {
		//�O�̃t���[�������A�j���[�V�������Ԃ��������Ȃ���(���[�v����)
		//�c���Ă���C�x���g��S�����������Ă��珉�����B
		AnimationEventGroupEx& eventGroupEx = eventGroupExTbl[currentAnimNo];
		for (
			int i = 0;
			eventGroupEx.eventGroup.event[i].eventType != eAnimationEventType_Invalid;
			i++
			) {
			if (eventGroupEx.invokeFlags[i] == false) {
				InvokeAnimationEvent(eventGroupEx.eventGroup.event[i]);
				//�����ς݂̈�B
				eventGroupEx.invokeFlags[i] = true;
			}
		}
		memset(eventGroupExTbl[currentAnimNo].invokeFlags, 0, sizeof(eventGroupExTbl[currentAnimNo].invokeFlags));
	}
	lastFrameAnimTime = animTime;
	AnimationEventGroupEx& eventGroupEx = eventGroupExTbl[currentAnimNo];
	for (
		int i = 0;
		eventGroupEx.eventGroup.event[i].eventType != eAnimationEventType_Invalid;
		i++
		) {
		if (eventGroupEx.invokeFlags[i] == false) {
			//�܂��C�x���g���������Ă��Ȃ��B
			if (eventGroupEx.eventGroup.event[i].time <= animTime) {
				//�C�x���g�������鎞�Ԃ��o�߂����B
				InvokeAnimationEvent(eventGroupEx.eventGroup.event[i]);
				//�����ς݂̈�B
				eventGroupEx.invokeFlags[i] = true;
			}
		}
	}
	animNoLastFrame = currentAnimNo;
	
}
