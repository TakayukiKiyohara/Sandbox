/*!
 * @brief	�G�̊��N���X�B
 */
 
#pragma once

#include "tkEngine/character/tkCharacterController.h"
#include "AnimationEventController.h"

struct EnemyParam;
class Enemy : public IGameObject{
public:
	enum EnAnimation {
		enAnimStand,
		enAnimWalk,
		enAnimAttack,
		enAnimDamage,
		enAnimDeath,
		enNumAnim,
		enAnimInvalid = 0xffffffff
	};
	Enemy() :
		currentAnimNo(enAnimInvalid),
		moveSpeed(0.0f),
		moveDirection(CVector3::AxisZ)
	{
	}
	virtual ~Enemy()
	{
	}
	virtual void Init(const char* modelPath, CVector3 pos, CQuaternion rotation);
	void Update() override;
	void PlayAnimation(EnAnimation animNo)
	{
		if(currentAnimNo != animNo)
		{
			animation.PlayAnimation(animNo, 0.3f);
			currentAnimNo = animNo;
		}
	}
	//�����A�j���[�V�������Đ�����Ă��Ă��C�ɂ����ɂł��ŏ����痬���B
	void PlayAnimationForce(EnAnimation animNo)
	{
		animation.PlayAnimation(animNo, 0.3f);
		currentAnimNo = animNo;
	}
	void SetMoveSpeed(float moveSpeed)
	{
		this->moveSpeed = moveSpeed;
	}
	const CVector3& GetMoveDirection() const
	{
		return moveDirection;
	}
	void SetMoveDirection( const CVector3& dir )
	{
		moveDirection = dir;
	}
	void SetDirection(const CVector3& dir)
	{
		direction = dir;
	}
	const CVector3& GetPosition() const
	{
		return position;
	}
	bool IsPlayAnimation() const
	{
		return animation.IsPlay();
	}
	/*!
	* @brief	���̃��[���h�s�������
	*@details
	* ���O�Ō������s���Ă��邽�߁A�x���ł��B�p�ɂɌĂ΂Ȃ��悤�ɁB
	*@param[in]	boneName	���̖��O�B
	*@return ���������獜�̃��[���h�s��̃A�h���X��Ԃ��B������Ȃ������ꍇ��NULL��Ԃ��B
	*/
	CMatrix* FindBoneWorldMatrix(const char* boneName)
	{
		return skinModel.FindBoneWorldMatrix(boneName);
	}
	/*!
	* @brief	�G�̃p�����[�^���擾�B
	*/
	const EnemyParam* GetEnemyParam() const
	{
		return enemyParam;
	}
	/*!
	* @brief	���S�������Ƃ�ʒm�B
	*/
	void NotifyDead();
protected:
	CSkinModelDataHandle			skinModelData;
	CSkinModel						skinModel;			//�X�L�����f���B
	CAnimation						animation;			//�A�j���[�V�����B
	CLight							light;				//���C�g�B
	CVector3						position;			//�ʒu
	CQuaternion						rotation;			//��]�B
	EnAnimation						currentAnimNo;		//���݂̃A�j���[�V�����ԍ��B
	float							moveSpeed;			//�ړ����x�B
	CVector3						moveDirection;		//�i�s�����B
	CVector3						direction;			//�����B
	int								hp;					//�̗́B
	std::vector<CTexture>			specMapList;		//�X�y�L�����}�b�v�B
	std::vector<CTexture>			normalMapList;		//�@���}�b�v�B
	const EnemyParam*				enemyParam;			//�G�̃p�����[�^�B
	CVector3						initPosition;			//�����ʒu�B
	AnimationEventController		animationEventController;	//�A�j���[�V�����C�x���g�R���g���[���[�B
	CCharacterController			characterController;	//�L�����N�^�R���g���[���B
	float							height;
	float							radius;
	float							timer = 0.0f;
};