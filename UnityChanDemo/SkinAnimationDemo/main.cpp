#include "stdafx.h"

#include "UnityChan.h"
#include "UnityChanInstance.h"
#include "Car.h"
#include "Map.h"
#include "Sky.h"
#include "Ground.h"

//#define MEMORY_LEAK_TEST		//定義でメモリリークテストが有効になる。

#ifdef MEMORY_LEAK_TEST
//メモリリークテスト。
class MemoryLeakTest : public IGameObject {
public:
	MemoryLeakTest()
	{

	}
	void Start() override
	{

	}
	void Update() override
	{
		//スキンなしモデル。
		CSkinModelData	nonSkinModelData;		//スキンモデルデータ。
		nonSkinModelData.LoadModelData("Assets/modelData/Court.X", NULL);
		//スキンなしインスタンシングモデル。
		CSkinModelData nonSkinModelInstancing;
		nonSkinModelInstancing.LoadModelData("Assets/modelData/Court.X", NULL);
		//インスタンス描画用のデータを作成。
		tkEngine::SVertexElement vertexElement[] = {
			{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },  // WORLD 1行目
			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },  // WORLD 2行目
			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },  // WORLD 3行目
			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },  // WORLD 4行目
			D3DDECL_END()
		};
		nonSkinModelInstancing.CreateInstancingDrawData(10, vertexElement);
		//スキンありモデル。
		CSkinModelData skinModelData;
		skinModelData.LoadModelData("Assets/modelData/Unity.X", NULL);
		//スキンありインスタンシングモデル。
		CSkinModelData skinModelInstancing;
		skinModelInstancing.LoadModelData("Assets/modelData/Unity.X", NULL);
		skinModelInstancing.CreateInstancingDrawData(10, vertexElement);

	}
	void Render(CRenderContext& renderContext) override
	{

	}
};
#endif


/*!
 * @brief	tkEngineの初期化。
 */
void InitTkEngine( HINSTANCE hInst )
{
	SInitParam initParam;
	memset(&initParam, 0, sizeof(initParam));
	//コマンドバッファのサイズのテーブル。
	int commandBufferSizeTbl[] = {
		10 * 1024 * 1024,		//10MB
	};
	initParam.hInstance = hInst;
	initParam.gameObjectPrioMax = 255;
	initParam.numRenderContext = 1;	//レンダリングコンテキストは一本
	initParam.commandBufferSizeTbl = commandBufferSizeTbl;
	initParam.screenHeight = 720;
	initParam.screenWidth = 1280;
	initParam.frameBufferHeight = 720;
	initParam.frameBufferWidth = 1280;
	initParam.graphicsConfig.bloomConfig.isEnable = true;
	initParam.graphicsConfig.edgeRenderConfig.isEnable = false;
	initParam.graphicsConfig.edgeRenderConfig.idMapWidth = initParam.frameBufferWidth;
	initParam.graphicsConfig.edgeRenderConfig.idMapHeight = initParam.frameBufferHeight;
	//Shadow
	initParam.graphicsConfig.shadowRenderConfig.isEnable = true;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapWidth = 2048;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapHeight = 2048;
	//reflection
	initParam.graphicsConfig.reflectionMapConfig.isEnable = false;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapWidth = 512;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapHeight = 512;
	//DOF
	initParam.graphicsConfig.dofConfig.isEnable = true;
	//AA
	initParam.graphicsConfig.aaConfig.isEnable = true;

	Engine().Init(initParam);	//初期化。
	
	ShadowMap().SetNear(2.0f);
	ShadowMap().SetFar(40.0f);
	
}

int WINAPI wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow
	)
{
	//tkEngineの初期化。
	InitTkEngine( hInst );
	
#ifdef MEMORY_LEAK_TEST
	NewGO<MemoryLeakTest>(0);
#else
	
	NewGO<UnityChanInstance>(0);
	NewGO<Map>(0);
	NewGO<Ground>(0);
	Sky* sky = NewGO<Sky>(0);
	UnityChan* unityChan = NewGO<UnityChan>(0);
	sky->SetUnityChan(unityChan);
	g_car = NewGO<Car>(0);
	g_camera = NewGO<GameCamera>(0);
	unityChan->SetPosition(CVector3(0.0f, 1.5f, 0.0f));
	g_camera->SetUnityChan(unityChan);
#endif
	Engine().RunGameLoop();		//ゲームループを実行。
	//地面との当たり判定のテスト。

	return 0;
}