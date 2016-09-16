/*!
 *@brief	車クラス。
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
	//乗車フラグの設定。
	void SetRideOnFlag(bool flag)
	{
		rideOnFlag = flag;
	}
	//進行方向を取得。
	const CVector3& GetMoveDirection() const
	{
		return moveDirection;
	}
	//移動速度を取得。
	const CVector3& GetMoveSpeed() const
	{
		return moveSpeed;
	}
private:
	CSkinModelData			skinModelData;
	CSkinModel				skinModel;			//スキンモデル。
	CLight					light;				//ライト。
	CTexture				normalMap;			//法線マップ。
	CTexture				speculerMap;		//スペキュラマップ。
	CVector3				position;			//座標。
	CQuaternion				rotation;			//回転。
	bool					rideOnFlag;			//乗車フラグ。
	CVector3				moveSpeed;			//移動速度。
	CVector3				accele;				//加速度。
	CVector3				moveDirection;		//進行方向。
};

extern Car* g_car;	//車。