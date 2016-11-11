/*!
 *@brief	車クラス
 */

#include "stdafx.h"
#include "Car.h"
#include "Player/Player.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"

Car* g_car;

Car::Car()
{
	skinModelData.LoadModelData("Assets/modelData/car.X", NULL);
	CSkinModelMaterial* mat = skinModelData.FindMaterial("Scout_Diffuse.png");
	normalMap.Load("Assets/modelData/Scout_Normal.png");
	speculerMap.Load("Assets/modelData/Scout_MetallicSmoothness.png");
	mat->SetTexture("g_normalTexture", &normalMap);
	mat->SetTexture("g_speculerMap", &speculerMap);
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	skinModel.SetHasNormalMap(true);
	skinModel.SetHasSpeculerMap(true);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	skinModel.SetReflectionCasterFlag(true);

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));

	light.SetDiffuseLightColor(0, CVector4(0.3f, 0.3f, 0.3f, 10.0f));
	light.SetDiffuseLightColor(1, CVector4(0.3f, 0.3f, 0.3f, 10.0f));
	light.SetDiffuseLightColor(2, CVector4(0.3f, 0.3f, 0.3f, 10.0f));
	light.SetDiffuseLightColor(3, CVector4(0.3f, 0.3f, 0.3f, 10.0f));
	light.SetAmbinetLight(CVector3(0.1f, 0.1f, 0.1f));
	position.Set(-12.0f, 3.5f, 0.0f);
	moveSpeed = CVector3::Zero;
	accele = CVector3::Zero;
	rotation = CQuaternion::Identity;
	moveDirection = CVector3::AxisZ;
	rideOnFlag = false;
	skinModel.SetFogParam(enFogFuncDist, 40.0f, 80.0f);
}
Car::~Car()
{

}
void Car::Start()
{

}
void Car::Update()
{
	if (rideOnFlag) {
		const CMatrix& mWorld = skinModel.GetWorldMatrix();
		moveDirection = CVector3(mWorld.m[2][0], mWorld.m[2][1], mWorld.m[2][2]);
		float moveSpeedScalar = moveSpeed.Length();

		//乗車中。
		if (Pad(0).IsPress(enButtonA)) {
			//車の進行方法に対して加速度をかける。
			accele = moveDirection;
			accele.Scale(2.0f);
		}else if(moveSpeedScalar > 0.1f && moveDirection.Dot(moveSpeed) > 0.0f){
			if (Pad(0).IsPress(enButtonB)) {
				//フットブレーキ
				accele = moveDirection;
				accele.Scale(-10.0f);
			}
			else {
				//エンジンブレーキ
				accele = moveDirection;
				accele.Scale(-3.0f);
			}
		}
		else {
			//完全停止。
			accele = CVector3::Zero;
			moveSpeed = CVector3::Zero;
		}

		float lstickX = Pad(0).GetLStickXF() * moveSpeed.Length();
		CQuaternion addRot;
		addRot.SetRotation(CVector3::AxisY, 0.01f * lstickX);
		rotation.Multiply(addRot);
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(addRot);
		mRot.Mul(moveSpeed);
		mRot.Mul(moveDirection);
		moveDirection.Normalize();
	}
	else {
		moveSpeed = CVector3::Zero;
		accele = CVector3::Zero;
	}
	CVector3 addSpeed = accele;
	addSpeed.Scale(1.0f / 60.0f);
	moveSpeed.Add(addSpeed);
	CVector3 addPos = moveSpeed;
	addPos.Scale(1.0f / 60.0f);
	position.Add(addPos);
	skinModel.Update(position, rotation, CVector3::One);
	light.SetPointLightPosition(g_player->GetPointLightPosition());
	light.SetPointLightColor(g_player->GetPointLightColor());
}
void Car::Render(CRenderContext& renderContext)
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}