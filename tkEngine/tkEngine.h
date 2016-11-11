/*!
 * @brief	tkEngine
 */
 

#ifndef _TKENGINE_H_
#define _TKENGINE_H_

#include "tkEngine/graphics/tkRenderContext.h"
#include "tkEngine/graphics/tkEffectManager.h"
#include "tkEngine/graphics/tkRenderTarget.h"
#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/preRender/tkPreRender.h"
#include "tkEngine/graphics/postEffect/tkPostEffect.h"
#include "tkEngine/graphics/tkGraphicsConfig.h"
#include "tkEngine/particle/tkParticleResources.h"
#include "tkEngine/resource/tkSkinModelDataResources.h"
#include "tkEngine/Sound/tkSoundEngine.h"
#include "tkEngine/Physics/tkPhysics.h"

namespace tkEngine{
	class CReflectionMap;
	/*!
	* @brief	描画優先ごとに対応するレンダリングコンテキストの番号のマップを定義するための構造体。
	*/
	struct SRenderContextMap {
		unsigned char	startPrio;			//!<開始プライオリティ
		int renderContextNo;	//!<レンダリングコンテキストNo
	};

	/*!
	 * @brief	初期化用のパラメータ。
	 */
	struct SInitParam{
		SInitParam()
		{
			memset(this, 0, sizeof(SInitParam));
		}
		HINSTANCE 			hInstance;
		unsigned char 					gameObjectPrioMax;		//!<ゲームオブジェクトの優先度の最大値(255まで)
		int 				numRenderContext;		//!<レンダリングコンテキストの数。この数が2以上の場合、renderContextMapの指定が必ず必要になります。
		int*				commandBufferSizeTbl;	//!<コマンドバッファのサイズのテーブル。レンダリングコンテキストのサイズ分必要。
		SRenderContextMap*	renderContextMap;		//!<描画優先ごとに対応するレンダリングコンテキストの番号のマップ。numRenderContextが1の場合は無視される。
		int					screenWidth;			//!<スクリーンの幅。
		int					screenHeight;			//!<スクリーンの高さ。
		int					frameBufferWidth;		//!<フレームバッファの幅。これが内部解像度。
		int					frameBufferHeight;		//!<フレームバッファの高さ。これが内部解像度。
		SGraphicsConfig		graphicsConfig;			//!<描画コンフィグ。
	};
	
	/*!
	 * @brief	エンジン。
	 */
	class CEngine : Noncopyable {
		CEngine() : 
			m_hWnd(nullptr),
			m_pD3D(nullptr),
			m_pD3DDevice(nullptr),
			m_frameBufferHeight(0),
			m_frameBufferWidth(0),
			m_screenHeight(0),
			m_screenWidth(0),
			m_numRenderContext(0),
			m_pTransformedPrimEffect(nullptr),
			m_currentMainRenderTarget(0)
		{}
		~CEngine() {}
	public:
		/*!
		 * @brief	初期化。
		 *@param[in]	初期化パラメータ
		 *@retval	true	初期化に成功。
		 *@retval	false	初期化に失敗。
		 */
		bool Init( const SInitParam& initParam );
		/*!
		 * @brief	終了処理。
		 */
		void Final();
		/*!
		 * @brief	ゲームループを実行。
		 */
		void RunGameLoop();
		/*!
		 * @brief	インスタンスの取得。
		 */
		static CEngine& Instance()
		{
			static CEngine* instance = nullptr;
			if (instance == nullptr) {
				instance = new CEngine;
			}
			return *instance;
		}
		/*!
		* @brief	エフェクトマネージャの取得。
		*/
		static CEffectManager& EffectManager()
		{
			return Instance().m_effectManager;
		}
		/*!
		 *@brief	Direct3DDeviceの取得。
		 */
		LPDIRECT3DDEVICE9 GetD3DDevice() const
		{
			return m_pD3DDevice;
		}
		/*!
		*@brief		スクリーンの幅を取得
		*/
		int GetScreenWidth() const
		{
			return m_screenWidth;
		}
		/*!
		*@brief		スクリーンの高さを取得
		*/
		int GetScreenHeight() const
		{
			return m_screenHeight;
		}
		/*!
		*@brief		フレームバッファの幅を取得。
		*/
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/*!
		*@brief		フレームバッファの高さを取得。
		*/
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/*!
		*@brief		IDMapの取得。
		*/
		CIDMap& IDMap()
		{
			return m_preRender.GetIDMap();
		}
		/*!
		*@brief		ShadowMapの取得。
		*/
		CShadowMap& ShadowMap()
		{
			return m_preRender.GetShadowMap();
		}
		/*!
		*@brief		リフレクションマップの取得。
		*/
		CReflectionMap& ReflectionMap()
		{
			return m_preRender.GetReflectionMap();
		}
		/*!
		* @brief	メインレンダリングターゲットを切り替え。
		*/
		void ToggleMainRenderTarget()
		{
			m_currentMainRenderTarget ^= 1;
		}
		/*!
		* @brief	メインレンダリングターゲットを取得。
		*/
		CRenderTarget& GetMainRenderTarget()
		{
			return m_mainRenderTarget[m_currentMainRenderTarget];
		}
		/*!
		* @brief	キー入力の取得。
		*/
		CKeyInput& KeyInput()
		{
			return m_keyInput;
		}
		/*!
		* @brief	パーティクルリソースの取得。
		*/
		CParticleResources& ParticleResource()
		{
			return m_particleResource;
		}
		/*!
		* @brief	DOFを取得。
		*/
		CDof& GetDof()
		{
			return m_postEffect.GetDof();
		}
		/*!
		* @brief	モーションブラーを取得。
		*/
		CMotionBlur& GetMotionBlur()
		{
			return m_postEffect.GetMotionBlur();
		}
		/*!
		* @brief	スキンモデルリソースを取得。
		*/
		CSkinModelDataResources& GetSkinModelDataResources()
		{
			return m_skinModelDataResources;
		}
		/*!
		* @brief	サウンドエンジンの取得。
		*/
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
		/*!
		* @brief	物理ワールドの取得。
		*/
		CPhysicsWorld& GetPhysicsWorld()
		{
			return m_physicsWorld;
		}
	private:
		/*!
		* @brief	メインレンダリングターゲットの内容をバックバッファにコピー。
		*/
		void CopyMainRenderTargetToBackBuffer(CRenderContext& renderContext);
		/*!
		* @brief	ウィンドウ初期化。
		* @retval	true	初期化に成功。
		* @retval	false	初期化に失敗。
		*/
		bool InitWindow(const SInitParam& initParam);
		/*!
		* @brief	DirectXの初期化。
		* @retval	true	初期化に成功。
		* @retval	false	初期化に失敗。
		*/
		bool InitDirectX(const SInitParam& initParam);
		/*!
		* @brief	メインレンダーターゲットをバックバッファにコピーするときに使うプリミティブを初期化。
		*/
		void InitCopyBackBufferPrimitive();
		/*!
		* @brief	ウィンドウプロシージャ。
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND									m_hWnd;						//!<ウィンドウハンドル。
		LPDIRECT3D9								m_pD3D;						//!<D3DDevice
		LPDIRECT3DDEVICE9						m_pD3DDevice;				//!<Rendering device
		std::unique_ptr<CRenderContext[]>		m_renderContextArray;		//!<レンダリングコンテキスト
		int										m_numRenderContext;			//!<レンダリングコンテキストの数。
		std::unique_ptr<SRenderContextMap[]>	m_renderContextMap;			//!<レンダリングコンテキストのマップ。
		CEffectManager							m_effectManager;			//!<エフェクトマネージャ。
		int										m_screenWidth;				//!<スクリーンの幅。
		int										m_screenHeight;				//!<スクリーンの高さ。
		int										m_frameBufferWidth;			//!<フレームバッファの幅。これが内部解像度。
		int										m_frameBufferHeight;		//!<フレームバッファの高さ。これが内部解像度。
		unsigned char							m_currentMainRenderTarget;	//!<現在使用されているメインレンダーターゲット。
		CRenderTarget							m_mainRenderTarget[2];		//!<メインレンダリングターゲット
		CEffect*								m_pTransformedPrimEffect;	//!<トランスフォーム済みプリミティブを描画するためのエフェクト。
		CPrimitive								m_copyBackBufferPrim;		//!<メインレンダーターゲットをバックバッファにコピーするときに使うプリミティブ。
		CRenderTarget							m_backBufferRT;				//!<バックバッファのレンダリングターゲット。
		CPreRender								m_preRender;				//!<プリレンダー。
		CPostEffect								m_postEffect;				//!<ポストエフェクト。
		CKeyInput								m_keyInput;					//!<キー入力。
		CParticleResources						m_particleResource;			//!<パーティクルのリソース管理。
		CFont									m_fpsFont;					//!<FPSを表示するデバッグフォント。
		CSkinModelDataResources					m_skinModelDataResources;	//!<スキンモデルデータリソース。
		CSoundEngine							m_soundEngine;				//!<サウンドエンジン。
		CPhysicsWorld							m_physicsWorld;				//!<物理ワールド。
	};
	static inline CEngine& Engine()
	{
		return CEngine::Instance();
	}
	static inline CIDMap& IDMap()
	{
		return CEngine::Instance().IDMap();
	}
	static inline CShadowMap& ShadowMap()
	{
		return CEngine::Instance().ShadowMap();
	}
	static inline CReflectionMap& ReflectionMap()
	{
		return CEngine::Instance().ReflectionMap();
	}
	static inline CEffectManager& EffectManager()
	{
		return CEngine::Instance().EffectManager();
	}
	static inline CKeyInput& KeyInput()
	{
		return CEngine::Instance().KeyInput();
	}
	static inline const CPad& Pad(int padNo)
	{
		return KeyInput().GetPad(padNo);
	}
	static inline CParticleResources& ParticleResources()
	{
		return CEngine::Instance().ParticleResource();
	}
	static inline CDof& Dof()
	{
		return CEngine::Instance().GetDof();
	}
	static inline CMotionBlur& MotionBlur()
	{
		return CEngine::Instance().GetMotionBlur();
	}
	static inline CSkinModelDataResources& SkinModelDataResources()
	{
		return CEngine::Instance().GetSkinModelDataResources();
	}
	static inline CSoundEngine& SoundEngine()
	{
		return CEngine::Instance().GetSoundEngine();
	}
	static inline CPhysicsWorld& PhysicsWorld()
	{
		return CEngine::Instance().GetPhysicsWorld();
	}
}

#endif // _TKENGINE_TKENGINE_H_
