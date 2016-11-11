#include "stdafx.h"
#include "Map/MapChip.h"
#include "Player/Player.h"

MapChip::MapChip() :
	rootBoneMatrix(NULL)
{
}


MapChip::~MapChip()
{
}
void MapChip::Init(const std::vector<SMapChipLocInfo*>& mapChipLocInfoList)
{
	//まずはスキンモデルをロード。
	char modelPath[1024];
	sprintf(modelPath, "Assets/modelData/%s.X", mapChipLocInfoList[0]->modelName);
	SkinModelDataResources().Load(skinModelData, modelPath, NULL, true, (int)mapChipLocInfoList.size());
	skinModel.Init(skinModelData.GetBody());
	skinModel.SetLight(&light);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	char filePath[256];
	const std::vector<CSkinModelMaterial*> materials = skinModelData.GetBody()->GetSkinModelMaterials();
	specMapList.resize(materials.size());
	normalMapList.resize(materials.size());
	int i = 0;
	for (CSkinModelMaterial* mat : materials) {
		char work[256];
		strcpy(work, mat->GetMaterialName());
		strtok(work, ".");
		sprintf(filePath, "Assets/modelData/%s_n.png", work);
		if (normalMapList[i].Load(filePath)) {
			mat->SetTexture("g_normalTexture", &normalMapList[i]);
			skinModel.SetHasNormalMap(true);
		}
		sprintf(filePath, "Assets/modelData/%s_s.png", work);
		if (specMapList[i].Load(filePath)) {
			mat->SetTexture("g_speculerMap", &specMapList[i]);
			skinModel.SetHasSpeculerMap(true);
		}

		i++;
	}

	//ワールド行列のバッファを作成。
	worldMatrixBuffer.reset(new CMatrix[mapChipLocInfoList.size()]);
	meshCollider.reset(new CMeshCollider[mapChipLocInfoList.size()]);
	rigidBody.reset(new CRigidBody[mapChipLocInfoList.size()]);
	i = 0;
	for (auto& mapChiplLocInfo : mapChipLocInfoList) {
		CMatrix mTrans;
		CVector3 pos = mapChiplLocInfo->pos;

		mTrans.MakeTranslation(pos);
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(mapChiplLocInfo->rotation);
		worldMatrixBuffer[i].Mul(mRot, mTrans);
		
		i++;
	}
	//行列を更新。
	Update();
	rootBoneMatrix = skinModelData.GetBody()->GetRootBoneWorldMatrix();
	i = 0;
	for (auto& mapChiplLocInfo : mapChipLocInfoList) {
		CMatrix mWorld;
		mWorld.Mul(*rootBoneMatrix, worldMatrixBuffer[i]);
		meshCollider[i].CreateFromSkinModel(&skinModel, &mWorld);
		RigidBodyInfo rbInfo;
		rbInfo.collider = &meshCollider[i];
		rbInfo.mass = 0.0f;
		rigidBody[i].Create(rbInfo);
		PhysicsWorld().AddRigidBody(&rigidBody[i]);
		i++;
	}
	

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));

	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));
	skinModel.SetFogParam(enFogFuncDist, 70.0f, 100.0f);

}
void MapChip::Start()
{

}
void MapChip::Update()
{
	light.SetPointLightPosition(g_player->GetPointLightPosition());
	light.SetPointLightColor(g_player->GetPointLightColor());
	skinModel.UpdateInstancingDrawData(worldMatrixBuffer.get());
	skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void MapChip::Render(CRenderContext& renderContext)
{
	renderContext.SetRenderState(RS_CULLMODE, CULL_NONE);
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
	renderContext.SetRenderState(RS_CULLMODE, CULL_CCW);
}