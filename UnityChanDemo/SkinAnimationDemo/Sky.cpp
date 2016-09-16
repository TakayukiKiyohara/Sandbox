#include "stdafx.h"
#include "Sky.h"
#include "UnityChan.h"
Sky::Sky() :
	unity(nullptr)
{
	skinModelData.LoadModelData("Assets/modelData/Sky.X", NULL);
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	
	light.SetAmbinetLight(CVector3(1.2f, 1.0f, 1.0f));
	//高さフォグをかける。
	skinModel.SetFogParam(enFogFuncHeight, 100.0f, 0.0f);
}

void Sky::Update()
{
	CVector3 pos;
	pos.x = unity->GetPosition().x;
	pos.y = 0.0f;
	pos.z = unity->GetPosition().z;
	skinModel.Update(pos, CQuaternion::Identity, CVector3(1.0f, 1.0f, 1.0f));
}
void Sky::Render(CRenderContext& renderContext) 
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}
