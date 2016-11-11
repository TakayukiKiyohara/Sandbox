#pragma once

#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"

class UnityChanInstance :
	public IGameObject
{
public:
	UnityChanInstance();
	~UnityChanInstance();
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
private:
	CSkinModelDataHandle	skinModelData;
	CSkinModel				skinModel;			//スキンモデル。
	CAnimation				animation;			//アニメーション。
	CLight					light;				//ライト。
	int						currentAnimSetNo;
	CTexture				normalMap;			//法線マップ。
	CTexture				specMap;			//スペキュラマップ。
	CMatrix*				worldMatrixBuffer;

};

