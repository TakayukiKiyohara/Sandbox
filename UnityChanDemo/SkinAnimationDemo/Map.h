#pragma once

class MapChip;

/*!
 * @brief	マップクラス。
 */
class Map : public IGameObject {
public:
	
	Map();
	~Map();
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
private:
	std::vector<MapChip*> mapChipList;	//マップチップのリスト。
};
