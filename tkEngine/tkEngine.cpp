/*!
 * @brief	tkEngine
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/gameObject/tkGameObjectManager.h"
#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/Input/tkKeyInput.h"
#include "tkEngine/timer/tkStopwatch.h"


//#ifdef _DEBUG
#define USE_DISP_FPS
//#endif

namespace tkEngine{
	LRESULT CALLBACK CEngine::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_LBUTTONUP: {
			unsigned int x = LOWORD(lParam);
			unsigned int y = HIWORD(lParam);
			tkEngine::KeyInput().OnMouseLButtonUp(x, y);
		}break;
		case WM_DESTROY:
			Instance().Final();
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	bool CEngine::InitWindow(const SInitParam& initParam)
	{
		TK_ASSERT(initParam.screenHeight != 0, "screenHeight is zero");
		TK_ASSERT(initParam.screenWidth != 0, "screenWidth is zero");
		TK_ASSERT(initParam.gameObjectPrioMax != 0, "gameObjectPrioMax is zero");
		TK_ASSERT(initParam.numRenderContext != 0, "numRenderContext is zero");
		TK_ASSERT(initParam.commandBufferSizeTbl != nullptr, "commandBufferSizeTbl is null");

		m_screenHeight = initParam.screenHeight;
		m_screenWidth = initParam.screenWidth;
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
			GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
			TEXT("D3D Tutorial"), nullptr
		};
		RegisterClassEx(&wc);
		// Create the application's window
		m_hWnd = CreateWindow(TEXT("D3D Tutorial"), TEXT("D3D Tutorial 06: Meshes"),
			WS_OVERLAPPEDWINDOW, 0, 0, m_screenWidth, m_screenHeight,
			nullptr, nullptr, wc.hInstance, nullptr);

		return m_hWnd != nullptr;
	}
	bool CEngine::InitDirectX(const SInitParam& initParam)
	{
		if( nullptr == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ){
			//D3Dオブジェクトを作成できなかった。
	        return false;
	    }
		//使用できるマルチサンプリングの品質を調べる。
		DWORD quality;
		HRESULT hr = m_pD3D->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, FALSE,
			D3DMULTISAMPLE_4_SAMPLES, &quality
		);

	    D3DPRESENT_PARAMETERS d3dpp;
	    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    	d3dpp.Windowed = TRUE;
	    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	    d3dpp.EnableAutoDepthStencil = TRUE;
    	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.BackBufferWidth = initParam.frameBufferWidth;
		d3dpp.BackBufferHeight = initParam.frameBufferHeight;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		m_frameBufferWidth = initParam.frameBufferWidth;
		m_frameBufferHeight = initParam.frameBufferHeight;

    	// Create the D3DDevice
	    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
	                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
	                                      &d3dpp, &m_pD3DDevice ) ) )
	    {
	        return false;
	    }
		//バックバッファのレンダリングターゲットと深度ステンシルバッファを取得しておいて覚えておく。
		LPDIRECT3DSURFACE9 rt, depth;
		m_pD3DDevice->GetRenderTarget(0, &rt);
		m_pD3DDevice->GetDepthStencilSurface(&depth);
		m_backBufferRT.SetSurfaceDX(rt);
		m_backBufferRT.SetDepthSurfaceDX(depth);
		m_fpsFont.Init();

	    return true;
	    
	}
	bool CEngine::Init(const SInitParam& initParam)
	{
		//ウィンドウ初期化。
		if (!InitWindow(initParam)) {
			return false;
		}
		//DirectX初期化。
		if (!InitDirectX(initParam)) {
			return false;
		}
		//メインレンダリングターゲットを作成。
		for (int i = 0; i < 2; i++) {
			m_mainRenderTarget[i].Create(
				m_frameBufferWidth,
				m_frameBufferHeight,
				1,
				FMT_A16B16G16R16F,
				FMT_D24S8,
				MULTISAMPLE_NONE,
				0
			);
		}

		CGameObjectManager::Instance().Init( initParam.gameObjectPrioMax );
		InitCopyBackBufferPrimitive();
		//レンダリングコンテキストの初期化。
		{
			m_renderContextArray.reset(new CRenderContext[initParam.numRenderContext]);
			for (int i = 0; i < initParam.numRenderContext; i++) {
				m_renderContextArray[i].Init(m_pD3DDevice, initParam.commandBufferSizeTbl[i]);
			}
			m_numRenderContext = initParam.numRenderContext;
			if (m_numRenderContext > 1) {
				TK_ASSERT(initParam.renderContextMap != nullptr, "renderContextMap is nullptr!!!");
				m_renderContextMap.reset( new SRenderContextMap[m_numRenderContext]);
				memcpy(m_renderContextMap.get(), initParam.renderContextMap, sizeof(SRenderContextMap) * m_numRenderContext);
			}
		}
		//トランスフォーム済みプリミティブを描画するシェーダーをロード。
		m_pTransformedPrimEffect = m_effectManager.LoadEffect("Assets/presetShader/TransformedPrim.fx");
		//プリレンダリングを作成。
		m_preRender.Create( initParam.graphicsConfig );
		//ポストエフェクトをレンダリング。
		m_postEffect.Create( initParam.graphicsConfig );
		m_soundEngine.Init();
		m_physicsWorld.Init();
		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(m_hWnd);
		return true;
	}
	void CEngine::InitCopyBackBufferPrimitive()
	{
		static SShapeVertex_PT vertex[]{
			{
				-1.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.0f
			},
			{
				1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 0.0f
			},
			{
				-1.0f, -1.0f, 0.0f, 1.0f,
				0.0f, 1.0f
			},
			{
				1.0f, -1.0f, 0.0f, 1.0f,
				1.0f, 1.0f
			},
		};
		static unsigned short index[] = {
			0,1,2,3
		};
		m_copyBackBufferPrim.Create(
			CPrimitive::eTriangleStrip,
			4,
			sizeof(SShapeVertex_PT),
			scShapeVertex_PT_Element,
			vertex,
			4,
			eIndexFormat16,
			index
			);
	}
	void CEngine::CopyMainRenderTargetToBackBuffer(CRenderContext& renderContext)
	{
		CRenderTarget& rt = m_mainRenderTarget[m_currentMainRenderTarget];
		m_pTransformedPrimEffect->SetTechnique(renderContext, "TransformedPrim");
		m_pTransformedPrimEffect->Begin(renderContext);
		m_pTransformedPrimEffect->BeginPass(renderContext, 0);
		float offset[] = {
			0.5f / GetFrameBufferWidth() ,
			0.5f / GetFrameBufferHeight()
		};
	
		m_pTransformedPrimEffect->SetTexture(renderContext, "g_tex", rt.GetTexture());
		m_pTransformedPrimEffect->SetValue(renderContext, "g_offset", offset, sizeof(offset));
		m_pTransformedPrimEffect->CommitChanges(renderContext);
		renderContext.SetVertexDeclaration(m_copyBackBufferPrim.GetVertexDecl());
		renderContext.SetStreamSource(0, m_copyBackBufferPrim.GetVertexBuffer());
		renderContext.SetIndices(m_copyBackBufferPrim.GetIndexBuffer());
		renderContext.DrawIndexedPrimitive(&m_copyBackBufferPrim);
		
		m_pTransformedPrimEffect->EndPass(renderContext);
		m_pTransformedPrimEffect->End(renderContext);
	}

	void CEngine::RunGameLoop()
	{
		// Enter the message loop
		
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
#ifdef USE_DISP_FPS
		char text[256] = {"\0"};
#endif
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				CStopwatch sw;
				sw.Start();
				m_keyInput.Update();
				m_skinModelDataResources.Update();
				m_physicsWorld.Update();
				m_soundEngine.Update();

				CRenderContext& topRenderContext = m_renderContextArray[0];
				CRenderContext& lastRenderContext = m_renderContextArray[m_numRenderContext - 1];

				topRenderContext.SetRenderTarget(0, &m_mainRenderTarget[m_currentMainRenderTarget]);
				//topRenderContext.SetRenderTarget(0, &m_backBufferRT);
				topRenderContext.SetRenderTarget(1, NULL);
				topRenderContext.SetRenderTarget(2, NULL);

				CGameObjectManager& goMgr = CGameObjectManager::Instance();
				goMgr.Execute(
					m_renderContextArray.get(), 
					m_numRenderContext, 
					m_renderContextMap.get(),
					m_preRender,
					m_postEffect
				);
				lastRenderContext.SetRenderTarget(0, &m_backBufferRT);
				lastRenderContext.Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0
				);
				CopyMainRenderTargetToBackBuffer(lastRenderContext);

				m_pD3DDevice->BeginScene();
				//レンダリングコマンドのサブミット
				for( int i = 0; i < m_numRenderContext; i++ ){
					m_renderContextArray[i].SubmitCommandBuffer();
				}

				//モーションブラーの更新。
				//1フレーム前のカメラを更新するので、全ての描画が完了したところで更新する。
				MotionBlur().Update();
#ifdef USE_DISP_FPS
				m_fpsFont.Draw(text, 0, 0);
#endif
				m_pD3DDevice->EndScene();
				m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);

				sw.Stop();
				
				if (sw.GetElapsed() < 1.0f / 30.0f) {
					//30fpsに間に合っているなら眠る。
					DWORD sleepTime = max( 0.0, (1.0 / 30.0)*1000.0 - (DWORD)sw.GetElapsedMillisecond());
					Sleep(sleepTime);
					GameTime().SetFrameDeltaTime(1.0f/30.0f);
				}
				else {
					//間に合っていない。
					GameTime().SetFrameDeltaTime((float)sw.GetElapsed());
				}
				//
#ifdef USE_DISP_FPS
				sprintf(text, "fps = %lf\n", 1.0f / sw.GetElapsed());
#endif


			}
		}
	}
	void CEngine::Final()
	{
		m_preRender.Release();
		m_postEffect.Release();
		m_backBufferRT.Release();
		for (int i = 0; i < 2; i++) {
			m_mainRenderTarget[i].Release();
		}
		m_effectManager.Release();
		if (m_pD3DDevice != nullptr)
			m_pD3DDevice->Release();

		if (m_pD3D != nullptr)
			m_pD3D->Release();
	}
}// namespace tkEngine
