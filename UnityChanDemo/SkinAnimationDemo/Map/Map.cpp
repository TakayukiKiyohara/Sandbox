#include "stdafx.h"
#include "Map/Map.h"
#include "Map/MapChip.h"

using namespace std;
namespace {
	//�}�b�v�`�b�v�̔z�u���̃e�[�u���B
	SMapChipLocInfo mapChipLocInfoTable[] = {
		#include "locationInfo.h"
	};
}
Map::Map()
{

}
Map::~Map()
{
	for(auto& mapchip : mapChipList){
		delete mapchip;
	}
}
void Map::Start()
{
	//�z�u��񂩂�}�b�v���\�z
	int tableSize = sizeof(mapChipLocInfoTable)/sizeof(mapChipLocInfoTable[0]);
	map<const char*, vector<SMapChipLocInfo*>*> m;
	//�C���X�^���V���O�`����s�����߂ɁA�������O�̃��f�����W�߂�B
	for(int i = 0; i < tableSize; i++){
		auto& it = m.find(mapChipLocInfoTable[i].modelName);
		if (it != m.end()) {
			//���̖��O�̃��f���͓o�^�ς�
			it->second->push_back(&mapChipLocInfoTable[i]);
		}
		else {
			//�V�K
			vector<SMapChipLocInfo*>* newList = new vector<SMapChipLocInfo*>;
			newList->push_back(&mapChipLocInfoTable[i]);
			m.insert(std::pair< const char*, vector<SMapChipLocInfo*>*>(
				mapChipLocInfoTable[i].modelName,
				newList
			));
		}
	}
	for (auto& mapchipList : m) {
		//�}�b�v�`�b�v�𐶐�
		MapChip* mapChip = new MapChip;
		mapChip->Init(*mapchipList.second);
		mapChipList.push_back(mapChip);
		delete mapchipList.second;
	}
}
void Map::Update()
{
	for(auto& mapChip : mapChipList){
		mapChip->Update();
	}
}
void Map::Render(CRenderContext& renderContext)
{
	for(auto& mapChip : mapChipList){
		mapChip->Render(renderContext);
	}
}