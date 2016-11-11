#include "stdafx.h"
#include "UnityChanInstance.h"
#include "Player/Player.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"

namespace {
	const int NUM_INSTANCE = 10;
}
UnityChanInstance::UnityChanInstance() :
	worldMatrixBuffer(nullptr),
	currentAnimSetNo(0)
{
}


UnityChanInstance::~UnityChanInstance()
{
	delete[] worldMatrixBuffer;
}
void UnityChanInstance::Start()
{
	SkinModelDataResources().Load(
		skinModelData, 
		"Assets/modelData/Unity.X", 
		&animation, 
		true, 
		NUM_INSTANCE
	);
	
	worldMatrixBuffer = new CMatrix[NUM_INSTANCE];
	int gyou = 1;		//行。
	int retu = 0;		//列。
	int numRetu = 3;	//列数。
	int execRetu = 0;	//配置した列の数。
	for (int i = 0; i < NUM_INSTANCE; i++) {
		if (retu == 0) {
			worldMatrixBuffer[i].MakeTranslation(CVector3(-10.0f, 3.5f, -0.5f * gyou));
			retu = 1;
		}
		else {
			
			worldMatrixBuffer[i].MakeTranslation(CVector3(-10.0f + 0.5f * retu, 3.5f, -0.5f * gyou));
			retu *= -1;
			if (retu > 0) {
				//正
				retu++;
			}
		}
		execRetu++;
		if (execRetu == numRetu) {
			//この行に配置できる列は全て配置した。
			execRetu = 0;
			retu = 0;
			gyou++;
			numRetu += 2;
		}
	}
	normalMap.Load("Assets/modelData/utc_nomal.tga");
	specMap.Load("Assets/modelData/utc_spec.tga");
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("utc_all2.tga");
	mat->SetTexture("g_normalTexture", &normalMap);
	mat->SetTexture("utc_spec.tga", &specMap);
	skinModel.Init(skinModelData.GetBody());
	skinModel.SetLight(&light);
	skinModel.SetHasNormalMap(true);
	skinModel.SetHasSpeculerMap(true);
	skinModel.SetFresnelFlag(true);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));

	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));
	light.SetLimLightColor(CVector4(0.5f, 0.5f, 0.5f, 1.0f));
	light.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	animation.SetAnimationEndTime(2, 0.8);
	animation.PlayAnimation(0);
	skinModel.SetFogParam(enFogFuncDist, 40.0f, 80.0f);
}
void UnityChanInstance::Update()
{
	light.SetPointLightPosition(g_player->GetPointLightPosition());
	light.SetPointLightColor(g_player->GetPointLightColor());
	animation.Update(1.0f / 60.0f);
	skinModel.UpdateInstancingDrawData(worldMatrixBuffer);
	skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
	
}
void UnityChanInstance::Render(CRenderContext& renderContext)
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}