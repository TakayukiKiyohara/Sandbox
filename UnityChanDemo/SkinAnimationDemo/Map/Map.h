#pragma once

class MapChip;

/*!
 * @brief	�}�b�v�N���X�B
 */
class Map : public IGameObject {
public:
	
	Map();
	~Map();
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
private:

private:
	std::vector<MapChip*> mapChipList;	//�}�b�v�`�b�v�̃��X�g�B
};
