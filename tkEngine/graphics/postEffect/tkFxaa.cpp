/*!
 *@brief	FXAAを使用したアンチエイリアス
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkFxaa.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	CFxaa::CFxaa() :
		m_effect(nullptr),
		m_isEnable(false)
	{
	}
	CFxaa::~CFxaa()
	{
	}
	void CFxaa::Release()
	{

	}
	void CFxaa::Init(const SGraphicsConfig& config)
	{
		if(config.aaConfig.isEnable){
			m_isEnable = config.aaConfig.isEnable;
			m_effect = EffectManager().LoadEffect("Assets/presetShader/fxaa.fx");
		}
	}
	void CFxaa::Render(CRenderContext& renderContext, CPostEffect* postEffect)
	{
		CPIXPerfTag tag(renderContext, L"CFxaa::Render");
		if(m_isEnable){
			//アンチ有効。
			float texSize[] = {
				s_cast<float>(Engine().GetMainRenderTarget().GetWidth()),
				s_cast<float>(Engine().GetMainRenderTarget().GetHeight())
			};
			m_effect->SetTechnique(renderContext, "FXAA");
			m_effect->Begin(renderContext);
			m_effect->BeginPass(renderContext, 0);
			m_effect->SetTexture(renderContext, "g_scene", Engine().GetMainRenderTarget().GetTexture());
			m_effect->SetValue(renderContext, "g_sceneTexSize", texSize, sizeof(texSize));
			m_effect->CommitChanges(renderContext);
			//レンダリングターゲットを切り替え。
			Engine().ToggleMainRenderTarget();
			renderContext.SetRenderTarget(0, &Engine().GetMainRenderTarget());
			postEffect->RenderFullScreen(renderContext);
			m_effect->EndPass(renderContext);
			m_effect->End(renderContext);
			
		}
	}
}