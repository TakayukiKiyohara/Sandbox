/*!
 *@brief	�ԃN���X�B
 */
#pragma once

class Car : public IGameObject{
public:
	Car();
	~Car();
	void Start() override ;
	void Update() override ;
	void Render( CRenderContext& renderContext ) override;
	const CVector3& GetPosition() const
	{
		return position;
	}
	//��ԃt���O�̐ݒ�B
	void SetRideOnFlag(bool flag)
	{
		rideOnFlag = flag;
	}
	//�i�s�������擾�B
	const CVector3& GetMoveDirection() const
	{
		return moveDirection;
	}
	//�ړ����x���擾�B
	const CVector3& GetMoveSpeed() const
	{
		return moveSpeed;
	}
private:
	CSkinModelData			skinModelData;
	CSkinModel				skinModel;			//�X�L�����f���B
	CLight					light;				//���C�g�B
	CTexture				normalMap;			//�@���}�b�v�B
	CTexture				speculerMap;		//�X�y�L�����}�b�v�B
	CVector3				position;			//���W�B
	CQuaternion				rotation;			//��]�B
	bool					rideOnFlag;			//��ԃt���O�B
	CVector3				moveSpeed;			//�ړ����x�B
	CVector3				accele;				//�����x�B
	CVector3				moveDirection;		//�i�s�����B
};

extern Car* g_car;	//�ԁB