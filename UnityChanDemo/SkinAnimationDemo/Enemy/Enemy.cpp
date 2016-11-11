/*!
 *@brief	敵の基底クラス。
 */
#include "stdafx.h"
#include "Enemy/Enemy.h"
#include "tkEngine/Physics/tkPhysics.h"
#include "tkEngine/character/tkCollisionAttr.h"
#include "Enemy/EnemyParameter.h"


void Enemy::Init(const char* modelPath, CVector3 pos, CQuaternion rotation)
{
	for (const EnemyParam& param : g_enemyParamTable) {
		if (strcmp(param.name, modelPath) == 0) {
			enemyParam = &param;
			break;
		}
	}
	char filePath[1024];
	sprintf(filePath, "Assets/modelData/%s.x", modelPath);
	SkinModelDataResources().Load(
		skinModelData,
		filePath,
		&animation
		);
	//マテリアルを取得。
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

	skinModel.Init(skinModelData.GetBody());
	position = pos;
	initPosition = position;
	this->rotation = rotation;

	skinModel.SetLight(&light);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	skinModel.SetFresnelFlag(true);

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));

	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 1.0f));

	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));

	light.SetLimLightColor(CVector4(0.6f, 0.6f, 0.6f, 1.0f));
	light.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));
	radius = 0.6f;
	height = 0.3f;
	characterController.Init(radius, height, position);
	characterController.SetGravity(-18.8f);
	if (enemyParam->animationEventGroup != NULL) {
		animationEventController.Init(
			&skinModel, 
			&animation, 
			enemyParam->animationEventGroup, 
			enNumAnim
		);
	}
}
void Enemy::Update()
{
	animationEventController.Update();
}
/*!
* @brief	死亡したことを通知。
*/
void Enemy::NotifyDead()
{
	characterController.RemoveRigidBoby();
}