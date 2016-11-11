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
	* @brief	�`��D�悲�ƂɑΉ����郌���_�����O�R���e�L�X�g�̔ԍ��̃}�b�v���`���邽�߂̍\���́B
	*/
	struct SRenderContextMap {
		unsigned char	startPrio;			//!<�J�n�v���C�I���e�B
		int renderContextNo;	//!<�����_�����O�R���e�L�X�gNo
	};

	/*!
	 * @brief	�������p�̃p�����[�^�B
	 */
	struct SInitParam{
		SInitParam()
		{
			memset(this, 0, sizeof(SInitParam));
		}
		HINSTANCE 			hInstance;
		unsigned char 					gameObjectPrioMax;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l(255�܂�)
		int 				numRenderContext;		//!<�����_�����O�R���e�L�X�g�̐��B���̐���2�ȏ�̏ꍇ�ArenderContextMap�̎w�肪�K���K�v�ɂȂ�܂��B
		int*				commandBufferSizeTbl;	//!<�R�}���h�o�b�t�@�̃T�C�Y�̃e�[�u���B�����_�����O�R���e�L�X�g�̃T�C�Y���K�v�B
		SRenderContextMap*	renderContextMap;		//!<�`��D�悲�ƂɑΉ����郌���_�����O�R���e�L�X�g�̔ԍ��̃}�b�v�BnumRenderContext��1�̏ꍇ�͖��������B
		int					screenWidth;			//!<�X�N���[���̕��B
		int					screenHeight;			//!<�X�N���[���̍����B
		int					frameBufferWidth;		//!<�t���[���o�b�t�@�̕��B���ꂪ�����𑜓x�B
		int					frameBufferHeight;		//!<�t���[���o�b�t�@�̍����B���ꂪ�����𑜓x�B
		SGraphicsConfig		graphicsConfig;			//!<�`��R���t�B�O�B
	};
	
	/*!
	 * @brief	�G���W���B
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
		 * @brief	�������B
		 *@param[in]	�������p�����[�^
		 *@retval	true	�������ɐ����B
		 *@retval	false	�������Ɏ��s�B
		 */
		bool Init( const SInitParam& initParam );
		/*!
		 * @brief	�I�������B
		 */
		void Final();
		/*!
		 * @brief	�Q�[�����[�v�����s�B
		 */
		void RunGameLoop();
		/*!
		 * @brief	�C���X�^���X�̎擾�B
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
		* @brief	�G�t�F�N�g�}�l�[�W���̎擾�B
		*/
		static CEffectManager& EffectManager()
		{
			return Instance().m_effectManager;
		}
		/*!
		 *@brief	Direct3DDevice�̎擾�B
		 */
		LPDIRECT3DDEVICE9 GetD3DDevice() const
		{
			return m_pD3DDevice;
		}
		/*!
		*@brief		�X�N���[���̕����擾
		*/
		int GetScreenWidth() const
		{
			return m_screenWidth;
		}
		/*!
		*@brief		�X�N���[���̍������擾
		*/
		int GetScreenHeight() const
		{
			return m_screenHeight;
		}
		/*!
		*@brief		�t���[���o�b�t�@�̕����擾�B
		*/
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/*!
		*@brief		�t���[���o�b�t�@�̍������擾�B
		*/
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/*!
		*@brief		IDMap�̎擾�B
		*/
		CIDMap& IDMap()
		{
			return m_preRender.GetIDMap();
		}
		/*!
		*@brief		ShadowMap�̎擾�B
		*/
		CShadowMap& ShadowMap()
		{
			return m_preRender.GetShadowMap();
		}
		/*!
		*@brief		���t���N�V�����}�b�v�̎擾�B
		*/
		CReflectionMap& ReflectionMap()
		{
			return m_preRender.GetReflectionMap();
		}
		/*!
		* @brief	���C�������_�����O�^�[�Q�b�g��؂�ւ��B
		*/
		void ToggleMainRenderTarget()
		{
			m_currentMainRenderTarget ^= 1;
		}
		/*!
		* @brief	���C�������_�����O�^�[�Q�b�g���擾�B
		*/
		CRenderTarget& GetMainRenderTarget()
		{
			return m_mainRenderTarget[m_currentMainRenderTarget];
		}
		/*!
		* @brief	�L�[���͂̎擾�B
		*/
		CKeyInput& KeyInput()
		{
			return m_keyInput;
		}
		/*!
		* @brief	�p�[�e�B�N�����\�[�X�̎擾�B
		*/
		CParticleResources& ParticleResource()
		{
			return m_particleResource;
		}
		/*!
		* @brief	DOF���擾�B
		*/
		CDof& GetDof()
		{
			return m_postEffect.GetDof();
		}
		/*!
		* @brief	���[�V�����u���[���擾�B
		*/
		CMotionBlur& GetMotionBlur()
		{
			return m_postEffect.GetMotionBlur();
		}
		/*!
		* @brief	�X�L�����f�����\�[�X���擾�B
		*/
		CSkinModelDataResources& GetSkinModelDataResources()
		{
			return m_skinModelDataResources;
		}
		/*!
		* @brief	�T�E���h�G���W���̎擾�B
		*/
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
		/*!
		* @brief	�������[���h�̎擾�B
		*/
		CPhysicsWorld& GetPhysicsWorld()
		{
			return m_physicsWorld;
		}
	private:
		/*!
		* @brief	���C�������_�����O�^�[�Q�b�g�̓��e���o�b�N�o�b�t�@�ɃR�s�[�B
		*/
		void CopyMainRenderTargetToBackBuffer(CRenderContext& renderContext);
		/*!
		* @brief	�E�B���h�E�������B
		* @retval	true	�������ɐ����B
		* @retval	false	�������Ɏ��s�B
		*/
		bool InitWindow(const SInitParam& initParam);
		/*!
		* @brief	DirectX�̏������B
		* @retval	true	�������ɐ����B
		* @retval	false	�������Ɏ��s�B
		*/
		bool InitDirectX(const SInitParam& initParam);
		/*!
		* @brief	���C�������_�[�^�[�Q�b�g���o�b�N�o�b�t�@�ɃR�s�[����Ƃ��Ɏg���v���~�e�B�u���������B
		*/
		void InitCopyBackBufferPrimitive();
		/*!
		* @brief	�E�B���h�E�v���V�[�W���B
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND									m_hWnd;						//!<�E�B���h�E�n���h���B
		LPDIRECT3D9								m_pD3D;						//!<D3DDevice
		LPDIRECT3DDEVICE9						m_pD3DDevice;				//!<Rendering device
		std::unique_ptr<CRenderContext[]>		m_renderContextArray;		//!<�����_�����O�R���e�L�X�g
		int										m_numRenderContext;			//!<�����_�����O�R���e�L�X�g�̐��B
		std::unique_ptr<SRenderContextMap[]>	m_renderContextMap;			//!<�����_�����O�R���e�L�X�g�̃}�b�v�B
		CEffectManager							m_effectManager;			//!<�G�t�F�N�g�}�l�[�W���B
		int										m_screenWidth;				//!<�X�N���[���̕��B
		int										m_screenHeight;				//!<�X�N���[���̍����B
		int										m_frameBufferWidth;			//!<�t���[���o�b�t�@�̕��B���ꂪ�����𑜓x�B
		int										m_frameBufferHeight;		//!<�t���[���o�b�t�@�̍����B���ꂪ�����𑜓x�B
		unsigned char							m_currentMainRenderTarget;	//!<���ݎg�p����Ă��郁�C�������_�[�^�[�Q�b�g�B
		CRenderTarget							m_mainRenderTarget[2];		//!<���C�������_�����O�^�[�Q�b�g
		CEffect*								m_pTransformedPrimEffect;	//!<�g�����X�t�H�[���ς݃v���~�e�B�u��`�悷�邽�߂̃G�t�F�N�g�B
		CPrimitive								m_copyBackBufferPrim;		//!<���C�������_�[�^�[�Q�b�g���o�b�N�o�b�t�@�ɃR�s�[����Ƃ��Ɏg���v���~�e�B�u�B
		CRenderTarget							m_backBufferRT;				//!<�o�b�N�o�b�t�@�̃����_�����O�^�[�Q�b�g�B
		CPreRender								m_preRender;				//!<�v�������_�[�B
		CPostEffect								m_postEffect;				//!<�|�X�g�G�t�F�N�g�B
		CKeyInput								m_keyInput;					//!<�L�[���́B
		CParticleResources						m_particleResource;			//!<�p�[�e�B�N���̃��\�[�X�Ǘ��B
		CFont									m_fpsFont;					//!<FPS��\������f�o�b�O�t�H���g�B
		CSkinModelDataResources					m_skinModelDataResources;	//!<�X�L�����f���f�[�^���\�[�X�B
		CSoundEngine							m_soundEngine;				//!<�T�E���h�G���W���B
		CPhysicsWorld							m_physicsWorld;				//!<�������[���h�B
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
