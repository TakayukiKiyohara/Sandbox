#include "stdafx.h"

#include "UnityChan.h"
#include "UnityChanInstance.h"
#include "Car.h"
#include "Map.h"
#include "Sky.h"
#include "Ground.h"

//#define MEMORY_LEAK_TEST		//��`�Ń��������[�N�e�X�g���L���ɂȂ�B

#ifdef MEMORY_LEAK_TEST
//���������[�N�e�X�g�B
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
		//�X�L���Ȃ����f���B
		CSkinModelData	nonSkinModelData;		//�X�L�����f���f�[�^�B
		nonSkinModelData.LoadModelData("Assets/modelData/Court.X", NULL);
		//�X�L���Ȃ��C���X�^���V���O���f���B
		CSkinModelData nonSkinModelInstancing;
		nonSkinModelInstancing.LoadModelData("Assets/modelData/Court.X", NULL);
		//�C���X�^���X�`��p�̃f�[�^���쐬�B
		tkEngine::SVertexElement vertexElement[] = {
			{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },  // WORLD 1�s��
			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },  // WORLD 2�s��
			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },  // WORLD 3�s��
			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },  // WORLD 4�s��
			D3DDECL_END()
		};
		nonSkinModelInstancing.CreateInstancingDrawData(10, vertexElement);
		//�X�L�����胂�f���B
		CSkinModelData skinModelData;
		skinModelData.LoadModelData("Assets/modelData/Unity.X", NULL);
		//�X�L������C���X�^���V���O���f���B
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
 * @brief	tkEngine�̏������B
 */
void InitTkEngine( HINSTANCE hInst )
{
	SInitParam initParam;
	memset(&initParam, 0, sizeof(initParam));
	//�R�}���h�o�b�t�@�̃T�C�Y�̃e�[�u���B
	int commandBufferSizeTbl[] = {
		10 * 1024 * 1024,		//10MB
	};
	initParam.hInstance = hInst;
	initParam.gameObjectPrioMax = 255;
	initParam.numRenderContext = 1;	//�����_�����O�R���e�L�X�g�͈�{
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

	Engine().Init(initParam);	//�������B
	
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
	//tkEngine�̏������B
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
	Engine().RunGameLoop();		//�Q�[�����[�v�����s�B
	//�n�ʂƂ̓����蔻��̃e�X�g�B

	return 0;
}