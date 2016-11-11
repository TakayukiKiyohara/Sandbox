#pragma once

class Sky : public IGameObject {
	CSkinModelData	skinModelData;		//スキンモデルデータ。
	CSkinModel		skinModel;			//スキンモデル。
	CAnimation		animation;			//アニメーション。
	CLight			light;				//ライト。
	CTexture		normalMap;
	Player*			player;				//プレイヤ。
public:
	Sky();
	
	~Sky()
	{

	}
	void Start() override
	{

	}
	void SetPlayer(Player* player)
	{
		this->player = player;
	}
	void Update() override;
	
	void Render(CRenderContext& renderContext) override;
	
};
