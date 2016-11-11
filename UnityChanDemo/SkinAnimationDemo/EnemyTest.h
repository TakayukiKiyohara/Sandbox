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

/*!
 * @brief	�G�̃e�X�g�v���O�����B
 */
class EnemyTest : public IGameObject {
private:
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//�����B
		AnimationWalk,		//�����B
		AnimationAttack,	//�U��
	};
public:
	enum EnState {
		enStateRun,			//�����Ă���B
		enStateStand,		//�����~�܂��Ă���B
		enState_RideOnCar,	//�Ԃɏ���Ă���
	};
	bool					isUpdateAnim;		//

	EnemyTest() :
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
private:
	/*!
	* @brief	�|�C���g���C�g�̈ʒu���X�V�B
	*/
	void UpdatePointLightPosition();
	/*!
	* @brief	�Փˌ��o�Ɖ����B
	*@param[in]	nextPosition		���̍��W�B
	*/
	void CollisionDetectAndResolve( CVector3 nextPosition );
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
	CVector3				pointLightPosition;	//�|�C���g���C�g�̈ʒu�B
	CVector4				pointLightColor;	//�|�C���g���C�g�̃J���[�B
	CVector3				toLampLocalPos;		//�����v�̃��[�J�����W�B
	EnState					state;				//��ԁB
	EnState					lastFrameState;		//�O�̃t���[���̏�ԁB
	bool					isJump;				//�W�����v���t���O�B
	bool					isPointLightOn;		//�|�C���g���C�g�̃X�C�b�`�B
	CSphereCollider			collider;			//�{�b�N�X�R���C�_�[�B
	float					radius;				//���a�B
	CRigidBody				rigidBody;			//���́B
};