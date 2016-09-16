#include "stdafx.h"
#include "UnityChanInstance.h"

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
	skinModelData.LoadModelData("Assets/modelData/Unity.X", &animation);
	//�C���X�^���X�`��p�̃f�[�^���쐬�B
	tkEngine::SVertexElement vertexElement[] = {
		{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },  // WORLD 1�s��
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },  // WORLD 2�s��
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },  // WORLD 3�s��
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },  // WORLD 4�s��
		D3DDECL_END()
	};
	skinModelData.CreateInstancingDrawData(NUM_INSTANCE, vertexElement);
	worldMatrixBuffer = new CMatrix[NUM_INSTANCE];
	int gyou = 1;		//�s�B
	int retu = 0;		//��B
	int numRetu = 3;	//�񐔁B
	int execRetu = 0;	//�z�u������̐��B
	for (int i = 0; i < NUM_INSTANCE; i++) {
		if (retu == 0) {
			worldMatrixBuffer[i].MakeTranslation(CVector3(0.0f, 1.5f, -0.5f * gyou));
			retu = 1;
		}
		else {
			
			worldMatrixBuffer[i].MakeTranslation(CVector3(0.5f * retu, 1.5f, -0.5f * gyou));
			retu *= -1.0f;
			if (retu > 0) {
				//��
				retu++;
			}
		}
		execRetu++;
		if (execRetu == numRetu) {
			//���̍s�ɔz�u�ł����͑S�Ĕz�u�����B
			execRetu = 0;
			retu = 0;
			gyou++;
			numRetu += 2;
		}
	}
	normalMap.Load("Assets/modelData/utc_nomal.tga");
	specMap.Load("Assets/modelData/utc_spec.tga");
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	skinModel.SetNormalMap(&normalMap);
	skinModel.SetSpeculerMap(&specMap);
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
	animation.Update(1.0f / 60.0f);
	skinModel.UpdateInstancingDrawData(worldMatrixBuffer);
	skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
	
}
void UnityChanInstance::Render(CRenderContext& renderContext)
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}