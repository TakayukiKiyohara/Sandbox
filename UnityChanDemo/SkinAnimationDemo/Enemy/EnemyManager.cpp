#include "stdafx.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Enemy_00.h"

using namespace std;
namespace {
	struct SEnemyLocInfo {
		const char* modelName;		//���f���B
		CVector3	pos;			//���W�B
		CQuaternion	rotation;		//��]�B
	};
	SEnemyLocInfo enemyLocInfoTable[] = {
		#include "EnemyInfo.h"
	};
}
EnemyManager::EnemyManager()
{
}
EnemyManager::~EnemyManager()
{
}
void EnemyManager::Start()
{
	//�z�u��񂩂�G���\�z�B
	map<const char*, vector<SEnemyLocInfo*>*> m;
	//�������O�̃��f�����W�߂�B
	for (SEnemyLocInfo& info : enemyLocInfoTable) {
		Enemy_00* enemy = NewGO<Enemy_00>(0);
		enemy->Init(info.modelName, info.pos, info.rotation);
	}
}
void EnemyManager::Update()
{
}
void EnemyManager::Render(CRenderContext& renderContext) 
{
}
