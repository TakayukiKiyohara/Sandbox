#pragma once

#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"
#include "tkEngine/Physics/tkSphereCollider.h"
#include "tkEngine/Physics/tkRigidBody.h"
#include "tkEngine/character/tkCharacterController.h"
#include "AnimationEventController.h"

namespace tkEngine{
	class CParticleEmitter;
}


/*!
 * @brief	�v���C���[�B
 */
class Player : public IGameObject {
private:
	static const int NUM_BATTLE_SEAT = 8;		//�V�[�g�̐��B
public:
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//�����B
		AnimationWalk,		//�����B
		AnimationRun,		//����B
		AnimationJump,		//�W�����v�B
		AnimationAttack_Start,
		AnimationAttack_00 = AnimationAttack_Start,	//�U��00�B
		AnimationAttack_01,	//�U��01�B
		AnimationAttack_02,	//�U��02�B
		AnimationAttack_End = AnimationAttack_02,
		AnimationDamage,	//�_���[�W�B
		AnimationDeath,		//���S�B
		NumAnimation,		//�A�j���[�V�����̐��B
	};
	//�퓬�Ŏg�p����V�[�g
	struct SBattleSeat {
		bool		isUse;			//�g�p���t���O�B
		int			seatNo;			//�V�[�g�ԍ��B
		CVector3	localPosition;	//���[�J�����W�B
		CVector3	position;		//���[���h���W�B
	};
	enum EnState {
		enStateRun,			//�����Ă���B
		enStateStand,		//�����~�܂��Ă���B
		enState_Attack,		//�U���B
		enState_Damage,		//�_���[�W���󂯂Ă���B
		enState_Dead,		//���S�B
	};
	bool					isUpdateAnim;		//

	Player() :
		position(CVector3::Zero),
		isUpdateAnim(false)
	{
		memset(battleSeats, 0, sizeof(battleSeats));
	}
	void Start() override ;
	void Update() override ;
	void Render( CRenderContext& renderContext ) override;
	void SetPosition(const CVector3& position)
	{
		this->position = position;
	}
	const CVector3& GetPosition() const
	{
		return position;
	}
	/*!
	* @brief	��Ԃ��擾�B
	*/
	EnState GetState() const
	{
		return state;
	}
	/*!
	* @brief	�|�C���g���C�g�̈ʒu���擾�B
	*/
	const CVector3& GetPointLightPosition() const
	{
		return pointLightPosition;
	}
	/*!
	* @brief	�|�C���g���C�g�̐F���擾�B
	*/
	const CVector4& GetPointLightColor() const
	{
		return pointLightColor;
	}
	/*!
	* @brief	���g�p�̃V�[�g�������B
	*@param[in]	pos		���g�p�̃V�[�g�̒����炱�̍��W�Ɉ�ԋ߂��V�[�g��Ԃ��܂��B
	*@return		���g�p�̃V�[�g���Ȃ��ꍇ��NULL���Ԃ��Ă���B
	*/
	SBattleSeat* FindUnuseSeat(const CVector3& pos) ;
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
	* @brief	HP�̎擾�B
	*/
	int GetHP() const
	{
		return hp;
	}
	/*!
	* @brief	�ő�HP�̎擾�B
	*/
	int GetMaxHP() const
	{
		return maxHP;
	}
	/*!
	* @brief	MP�̎擾�B
	*/
	float GetMP() const
	{
		return mp;
	}
	/*!
	* @brief	�ő�MP�̎擾�B
	*/
	float GetMaxMP() const
	{
		return maxMP;
	}
private:
	/*!
	* @brief	��Ԑ؂�ւ��B
	*/
	void ChangeState(EnState nextState);
	/*!
	* @brief	�����������B
	*/
	void Damage();
	/*!
	* @brief	�|�C���g���C�g�̈ʒu���X�V�B
	*/
	void UpdatePointLightPosition();
	/*!
	* @brief	�A�j���[�V�����R���g���[���B
	*/
	void AnimationControl();
	/*!
	* @brief	�A�j���[�V�����Đ��B
	*/
	void PlayAnimation(AnimationNo animNo, float interpolateTime);
	/*!
	* @brief	�o�g���Ŏg�p����V�[�g���������B
	*/
	void InitBattleSeats();
	/*!
	* @brief	�o�g���Ŏg�p����V�[�g���X�V�B
	*/
	void UpdateBattleSeats();

	void UpdateStateMachine();
	/*!
	* @brief	�}�W�b�N�|�C���g���g�p����B
	*/
	void UseMagicPoint(float useMp)
	{
		mp = max(0, mp - useMp);
	}
	/*!
	* @brief	�}�W�b�N�|�C���g�̉񕜁B
	*/
	void RecoverMagicPoint(float recoverMp)
	{
		mp = min(maxMP, mp + recoverMp);
	}
private:
	CSkinModelDataHandle	skinModelData;
	CSkinModel				skinModel;			//�X�L�����f���B
	CAnimation				animation;			//�A�j���[�V�����B
	CLight					light;				//���C�g�B
	CTexture				normalMap;			//�@���}�b�v�B
	CTexture				specMap;			//�X�y�L�����}�b�v�B
	CTexture				weaponNormalMap;	//����̖@���}�b�v�B
	CTexture				weaponSpecMap;		//����̃X�y�L�����}�b�v�B
	CVector3				position;			//���W�B
	CQuaternion				rotation;			//��]
	CVector3				toLightPos;			//
	CVector3				pointLightPosition;	//�|�C���g���C�g�̈ʒu�B
	CVector4				pointLightColor;	//�|�C���g���C�g�̃J���[�B
	CVector3				toLampLocalPos;		//�����v�̃��[�J�����W�B
	bool					isApplyDamageTrigger = false;
	EnState					state;				//��ԁB
	EnState					lastFrameState;		//�O�̃t���[���̏�ԁB
	bool					isPointLightOn;		//�|�C���g���C�g�̃X�C�b�`�B
	CRigidBody				rigidBody;			//���́B
	CCharacterController	characterController;	//�L�����N�^�R���g���[���B
	SBattleSeat				battleSeats[NUM_BATTLE_SEAT];	//�V�[�g�B
	AnimationNo				reqAttackAnimNo;		//�Đ��̃��N�G�X�g���o���Ă���U�����[�V�����ԍ��B
	AnimationNo				nextAttackAnimNo;		//���̍U�����[�V�����ԍ��B
	AnimationEventController	animationEventController;	//�A�j���[�V�����C�x���g�R���g���[���B
	std::list<CParticleEmitter*>	particleEmitterList;
	int						hp =  100;					//�q�b�g�|�C���g�B
	int						maxHP = 100;				//�ő�q�b�g�|�C���g�B
	float					mp = 100.0f;				//�}�W�b�N�|�C���g�B
	float					maxMP = 100.0f;				//�ő�}�W�b�N�|�C���g�B
	float					radius = 0.0f;
	float					height = 0.0f;
	float					timer = 0.0f;				//�^�C�}�[
	float					magicPointRecoverTimer = 0.0f;		//�}�W�b�N�|�C���g�̉񕜃^�C�}�[�B
};