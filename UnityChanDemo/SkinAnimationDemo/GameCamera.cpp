#include "stdafx.h"
#include "GameCamera.h"
#include "UnityChan.h"
#include "Car.h"

GameCamera			*g_camera;				//カメラ。

GameCamera::GameCamera() :
	unityChan(nullptr),
	car(nullptr)
{
}
GameCamera::~GameCamera()
{
}
void GameCamera::Start()
{
	camera.SetPosition(CVector3(0.0f, 0.8f, 3.0f));
	camera.SetTarget(CVector3(0.0f, 0.2f, 0.0f));
	toPosition.Subtract(camera.GetPosition(), camera.GetTarget());
	camera.SetFar(1000.0f);
	camera.Update();
	//リフレクションマップにカメラを設定する。
	ReflectionMap().SetCamera(camera);
}
void GameCamera::Update()
{
	//カメラを回転させる。
	float rStick_x = Pad(0).GetRStickXF();
	float rStick_y = Pad(0).GetRStickYF();
	
	if (fabsf(rStick_x) > 0.0f) {
		//Y軸周りの回転を計算。
		//回す
		CMatrix mRot;
		mRot.MakeRotationY(0.05f * rStick_x);
		mRot.Mul(toPosition);
	}
	if (fabsf(rStick_y) > 0.0f) {
		CVector3 rotAxis;
		rotAxis.Cross(CVector3::Up, toPosition);
		rotAxis.Normalize();
		CMatrix mRot;
		mRot.MakeRotationAxis(rotAxis, 0.05f * rStick_y);
		CVector3 toPositionOld = toPosition;
		mRot.Mul(toPosition);
		CVector3 toPosDir = toPosition;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//カメラが上向きすぎ。
			toPosition = toPositionOld;
		}
		else if (toPosDir.y > 0.8f) {
			//カメラが下向きすぎ。
			toPosition = toPositionOld;
		}
	}
	CVector3 v;
	if (car) {
		//車の後ろに追従。
		v = car->GetPosition();
		v.y += 1.0f;
		camera.SetTarget(v);
		CVector3 toPosOnCar = car->GetMoveDirection();
		toPosOnCar.Scale(-3.0f);
		toPosOnCar.y += 1.0f;
		v.Add(toPosOnCar);
		camera.SetPosition(v);
	}
	else {
		v = unityChan->GetPosition();
		v.y += 1.0f;
		camera.SetTarget(v);
		v.Add(toPosition);
		camera.SetPosition(v);
		
	}
	camera.Update();
	//被写界深度のパラメータを更新
	Dof().SetFocalLength(28.0f);
	Dof().SetFParam(5.6f);
	Dof().SetPint(toPosition.Length() * 1000.0f);
}
void GameCamera::Render( CRenderContext& renderContext )
{
}