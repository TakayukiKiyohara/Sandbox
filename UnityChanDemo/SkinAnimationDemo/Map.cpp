#include "stdafx.h"
#include "Map.h"
#include "MapChip.h"

using namespace std;
namespace {
	//マップチップの配置情報のテーブル。
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
	//配置情報からマップを構築
	int tableSize = sizeof(mapChipLocInfoTable)/sizeof(mapChipLocInfoTable[0]);
	map<const char*, vector<SMapChipLocInfo*>*> m;
	//インスタンシング描画を行うために、同じ名前のモデルを集める。
	for(int i = 0; i < tableSize; i++){
		auto& it = m.find(mapChipLocInfoTable[i].modelName);
		if (it != m.end()) {
			//この名前のモデルは登録済み
			it->second->push_back(&mapChipLocInfoTable[i]);
		}
		else {
			//新規
			vector<SMapChipLocInfo*>* newList = new vector<SMapChipLocInfo*>;
			newList->push_back(&mapChipLocInfoTable[i]);
			m.insert(std::pair< const char*, vector<SMapChipLocInfo*>*>(
				mapChipLocInfoTable[i].modelName,
				newList
			));
		}
	}
	for (auto& mapchipList : m) {
		//マップチップを生成
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