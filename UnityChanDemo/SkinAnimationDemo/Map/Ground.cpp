#include "stdafx.h"
#include "tkEngine/Physics/tkPhysics.h"
#include "Map/Ground.h"
#include "Player/Player.h"
#include "tkEngine/character/tkCollisionAttr.h"

Ground* g_ground = NULL;
LPD3DXMESH testMesh;

void Ground::Start()
{
	g_ground = this;
	skinModelData.LoadModelData("Assets/modelData/ground.X", NULL);
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	CSkinModelMaterial* mat = skinModelData.FindMaterial("Grass.tga");
	normalMap.Load("Assets/modelData/Grass_Normals.tga");
	mat->SetTexture("g_normalTexture", &normalMap);
	skinModel.SetHasNormalMap(true);
	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));

	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));
	skinModel.SetShadowReceiverFlag(true);
	skinModel.SetShadowCasterFlag(true);
	//�����t�H�O��������B
	skinModel.SetFogParam(enFogFuncDist, 70.0f, 100.0f);

	Update();
	m_worldMatrix = skinModel.FindBoneWorldMatrix("Plane001");
	//���b�V���R���C�_�[���쐬�B
	meshCollider.CreateFromSkinModel(&skinModel, m_worldMatrix);
	//���̂��쐬�B
	RigidBodyInfo rbInfo;
	rbInfo.collider = &meshCollider;
	rbInfo.mass = 0.0f;
	rigidBody.Create(rbInfo);
	rigidBody.GetBody()->setUserIndex(enCollisionAttr_Ground);
	//���̂����[���h�ɒǉ��B
	PhysicsWorld().AddRigidBody(&rigidBody);
}

void Ground::Update() 
{
	light.SetPointLightPosition(g_player->GetPointLightPosition());
	light.SetPointLightColor(g_player->GetPointLightColor());
	skinModel.Update(CVector3(0.0f, 0.0f, 0.0f), CQuaternion::Identity, CVector3(1.0f, 1.0f, 1.0f));
}
void Ground::Render(CRenderContext& renderContext) 
{
	skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
}
void Ground::IsIntersect(const CVector3& pos, const CVector3& ray, int& bHit, float& len)
{
	CMatrix mWorldInv;
	mWorldInv.Inverse(*m_worldMatrix);
	CVector3 posInGround = pos;
	mWorldInv.Mul(posInGround);
	CVector3 rayInGround = ray;
	mWorldInv.Mul3x3(rayInGround);	//���C�͉񂷂����Ȃ̂�3x3�s�����Z����B
	//���C�͉񂷂���
	HRESULT hr = D3DXIntersect(
		skinModel.GetOrgMeshFirst(),		//�R�[�X���烁�b�V�������������Ă���B 
		(const D3DXVECTOR3*)&posInGround,	//�Ԃ̍��W�E�E�E�������A�R�[�X���W�n�B
		(const D3DXVECTOR3*)&rayInGround,	//���C�E�E�E�������A�R�[�X���W�n�B 
		&bHit,								//�Փ˂��Ă�����1�A�Փ˂��ĂȂ�������0 
		NULL, 
		NULL, 
		NULL, 
		&len,								//�n�_�����_�܂ł̋����B
		NULL, 
		NULL
	);
	if (FAILED(hr)) {
		TK_LOG("IsInterSect Error");
	}
}