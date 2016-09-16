#pragma once

#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"

/*!
 * @brief	���j�e�B�����
 */
class UnityChan : public IGameObject {
private:
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//�����B
		AnimationWalk,		//�����B
		AnimationRun,		//����B
		AnimationJump,		//�W�����v�B
	};
public:
	enum EnState {
		enStateRun,			//�����Ă���B
		enStateStand,		//�����~�܂��Ă���B
		enState_RideOnCar,	//�Ԃɏ���Ă���
	};
	bool					isUpdateAnim;		//

	UnityChan() :
		position(CVector3::Zero),
		isUpdateAnim(false)
	{

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
private:
	/*!
	* @brief	�Փˌ��o�Ɖ����B
	*@param[in]	nextPosition		���̍��W�B
	*/
	void CollisionDetectAndResolve( const CVector3& nextPosition );
	/*!
	* @brief	�A�j���[�V�����R���g���[���B
	*/
	void AnimationControl();
	/*!
	* @brief	�A�j���[�V�����Đ��B
	*/
	void PlayAnimation(AnimationNo animNo);
private:
	
	static CSkinModelData*	orgSkinModelData;		//�X�L�����f���f�[�^�B
	CSkinModelData			skinModelData;
	CSkinModel				skinModel;			//�X�L�����f���B
	CAnimation				animation;			//�A�j���[�V�����B
	CLight					light;				//���C�g�B
	AnimationNo				currentAnimSetNo;
	CTexture				normalMap;			//�@���}�b�v�B
	CTexture				specMap;			//�X�y�L�����}�b�v�B
	CVector3				position;			//���W�B
	CQuaternion				rotation;			//��]
	CVector3				toLightPos;			//
	CVector3				moveSpeed;			//�ړ����x�B
	EnState					state;				//��ԁB
	EnState					lastFrameState;		//�O�̃t���[���̏�ԁB
	bool					isJump;				//�W�����v���t���O�B
};