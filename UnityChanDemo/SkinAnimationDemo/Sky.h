#pragma once

class Sky : public IGameObject {
	CSkinModelData	skinModelData;		//スキンモデルデータ。
	CSkinModel		skinModel;			//スキンモデル。
	CAnimation		animation;			//アニメーション。
	CLight			light;				//ライト。
	CTexture		normalMap;
	UnityChan*		unity;				//ユニティちゃん
public:
	Sky();
	
	~Sky()
	{

	}
	void Start() override
	{

	}
	void SetUnityChan(UnityChan* unity)
	{
		this->unity = unity;
	}
	void Update() override;
	
	void Render(CRenderContext& renderContext) override;
	
};
