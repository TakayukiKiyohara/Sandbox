#pragma once
//マップチップの配置情報。
struct SMapChipLocInfo {
	const char* modelName;		//モデル。
	CVector3	pos;			//座標。
	CQuaternion	rotation;		//回転。
};
class MapChip : public IGameObject
{
public:
	MapChip();
	~MapChip();
	void Init(const std::vector<SMapChipLocInfo*>& mapChipLocInfoList);
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
private:
	CSkinModelData					skinModelData;
	CSkinModel						skinModel;			//スキンモデル。
	CAnimation						animation;			//アニメーション。
	CLight							light;				//ライト。
	CTexture						specMap;			//スペキュラマップ。
	std::unique_ptr<CMatrix[]>		worldMatrixBuffer;	//ワールド行列のバッファ
};

