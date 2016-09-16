#include "stdafx.h"
#include "MapChip.h"


MapChip::MapChip()
{
}


MapChip::~MapChip()
{
}
void MapChip::Init(const std::vector<SMapChipLocInfo*>& mapChipLocInfoList)
{
	//まずはスキンモデルをロード。
	char modelPath[256];
	sprintf(modelPath, "Assets/modelData/%s.X", mapChipLocInfoList[0]->modelName);
	skinModelData.LoadModelData(modelPath, NULL);
	//インスタンス描画用のデータを作成。
	tkEngine::SVertexElement vertexElement[] = {
		{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },  // WORLD 1行目
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },  // WORLD 2行目
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },  // WORLD 3行目
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },  // WORLD 4行目
		D3DDECL_END()
	};
	skinModelData.CreateInstancingDrawData(mapChipLocInfoList.size(), vertexElement);
	//ワールド行列のバッファを作成。
	worldMatrixBuffer.reset(new CMatrix[mapChipLocInfoList.size()]);
	int i = 0;
	for (auto& mapChiplLocInfo : mapChipLocInfoList) {
		CMatrix mTrans;
		CVector3 pos = mapChiplLocInfo->pos;
		pos.Scale(0.38f);
		mTrans.MakeTranslation(pos);
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(mapChiplLocInfo->rotation);
		worldMatrixBuffer[i].Mul(mRot, mTrans);
		i++;
	}
	
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
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
	light.SetAmbinetLight(CVector3(0.7f, 0.7f, 0.7f));
	skinModel.SetFogParam(enFogFuncDist, 40.0f, 80.0f);

}
void MapChip::Start()
{

}
void MapChip::Update()
{
	skinModel.UpdateInstancingDrawData(worldMatrixBuffer.get());
	skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void MapChip::Render(CRenderContext& renderContext)
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}