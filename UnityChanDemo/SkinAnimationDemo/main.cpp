#include "stdafx.h"

#include "Player/Player.h"
#include "UnityChanInstance.h"
#include "Car.h"
#include "Map/Map.h"
#include "Map/Sky.h"
#include "Map/Ground.h"
#include "tkEngine/Physics/tkPhysics.h"
#include "EnemyTest.h"
#include "Enemy/EnemyManager.h"
#include <time.h>
#include "DamageCollisionWorld.h"
#include "tkEngine/Sound/tkSoundSource.h"
#include "tkEngine/graphics/sprite/tkSprite.h"
#include "HUD/PlayerHPBar.h"
#include "HUD/PlayerMPBar.h"

CPhysicsWorld* g_physicsWorld = NULL;
Player* g_player = NULL;
CRandom g_random;
DamageCollisionWorld* g_damageCollisionWorld = NULL;
//#define MEMORY_LEAK_TEST		//定義でメモリリークテストが有効になる。
#define PLAY_WAVE_FILE_TEST		//定義で波形データの再生テストが有効になる。
//#define DRAW_SPRITE_TEST		//定義でスプライト描画テスト。

#ifdef DRAW_SPRITE_TEST
class DrawSpriteTest : public IGameObject {
	CSprite	sprite;
	CTexture texture;
public:
	DrawSpriteTest()
	{
	}
	~DrawSpriteTest()
	{
	}
	void Start() override
	{
		texture.Load("Assets/sprite/test.png");
		sprite.Init(&texture);
		sprite.SetPosition({ -640, 360 });
		sprite.SetPivot({ 0.0f, 1.0f });
	}
	void Update() override
	{
	}
	void PostRender( CRenderContext& renderContext ) override
	{
		sprite.Draw(renderContext);
	}
};
#endif
#ifdef PLAY_WAVE_FILE_TEST
class PlayWaveFileTest : public IGameObject {
	/*CWaveFile waveFile;
	char* buffer;*/
	CSoundSource soundSource;
public:
	PlayWaveFileTest() 
	{
	}
	~PlayWaveFileTest()
	{
	}
	void Start() override
	{
		//サウンドソースを初期化。
		soundSource.InitStreaming("Assets/sound/wind.wav");
		//soundSource.Init("Assets/sound/MusicMono.wav");
		soundSource.Play(true);
		soundSource.SetVolume(0.5f);
		AddGO(0, &soundSource);
	}
	void Update() override
	{
		/*if (Pad(0).IsPress(enButtonDown)) {
			CSoundSource* s = NewGO<CSoundSource>(0);
			s->Init("Assets/sound/EnemyAttack.wav", true);
			s->Play(false);
			s->SetPosition(g_player->GetPosition());
			s = NewGO<CSoundSource>(0);
			s->Init("Assets/sound/Damage_00.wav");
			s->Play(false);
		}*/
	}
	void Render(CRenderContext& renderContext) override
	{
	}
};
#endif
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
		CSkinModelDataHandle nonSkinModelData;		//スキンモデルデータ。
		SkinModelDataResources().Load(nonSkinModelData, "Assets/modelData/ground.X", NULL);
		
		//スキンなしインスタンシングモデル。
		CSkinModelDataHandle nonSkinModelInstancing;
		SkinModelDataResources().Load(nonSkinModelInstancing, "Assets/modelData/ground.X", NULL, true, 10);
		
		//スキンありモデル。
		CSkinModelDataHandle skinModelData;
		SkinModelDataResources().Load(skinModelData, "Assets/modelData/Unity.X", NULL);
		//スキンありインスタンシングモデル。
		CSkinModelDataHandle skinModelInstancing;
		SkinModelDataResources().Load(skinModelInstancing, "Assets/modelData/Unity.X", NULL, true, 10);

		CSoundSource soundSource;
		soundSource.InitStreaming("Assets/sound/SoundTest.wav");
		//soundSource.Init("Assets/sound/MusicMono.wav");
		soundSource.Play(true);

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
	//Bloom
	initParam.graphicsConfig.bloomConfig.isEnable = true;
	initParam.graphicsConfig.edgeRenderConfig.isEnable = false;
	initParam.graphicsConfig.edgeRenderConfig.idMapWidth = initParam.frameBufferWidth;
	initParam.graphicsConfig.edgeRenderConfig.idMapHeight = initParam.frameBufferHeight;
	//Shadow
	initParam.graphicsConfig.shadowRenderConfig.Init();
	initParam.graphicsConfig.shadowRenderConfig.isEnable = true;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapWidth = 1024;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapHeight = 1024;
	initParam.graphicsConfig.shadowRenderConfig.numShadowMap = 3;
	
	//reflection
	initParam.graphicsConfig.reflectionMapConfig.isEnable = false;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapWidth = 512;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapHeight = 512;
	//DOF
	initParam.graphicsConfig.dofConfig.isEnable = true;
	//AA
	initParam.graphicsConfig.aaConfig.isEnable = true;
	//MotionBlur
	initParam.graphicsConfig.motionBlurConfig.isEnable = true;

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
	g_random.Init((unsigned long)time(NULL));
#ifdef DRAW_SPRITE_TEST
	NewGO<DrawSpriteTest>(1);
#endif
#ifdef PLAY_WAVE_FILE_TEST
	NewGO<PlayWaveFileTest>(0);
#endif
#ifdef MEMORY_LEAK_TEST
	NewGO<MemoryLeakTest>(0);
#else

	g_player = NewGO<Player>(0);
	NewGO<UnityChanInstance>(0);
	NewGO<EnemyManager>(0);
	NewGO<Map>(0);
	NewGO<Ground>(0);
	NewGO<PlayerHPBar>(0);
	NewGO<PlayerMPBar>(0);
	g_damageCollisionWorld = NewGO<DamageCollisionWorld>(0);

	Sky* sky = NewGO<Sky>(0);
	sky->SetPlayer(g_player);
	g_car = NewGO<Car>(0);
	g_camera = NewGO<GameCamera>(0);
	g_player->SetPosition(CVector3(-10.0f, 4.5f, 0.0f));
	g_camera->SetPlayer(g_player);
	MotionBlur().SetCamera(&g_camera->GetCamera());
#endif
	Engine().RunGameLoop();		//ゲームループを実行。

	return 0;
}