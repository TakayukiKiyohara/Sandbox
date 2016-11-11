/*!
 * @brief	被写界深度。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkDOF.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CDof::CDof() :
		m_isEnable(false),
		m_pint(3000.0f),
		m_focalLength(24.0f),
		m_F(5.6f)
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CDof::~CDof()
	{
	}
	
	void CDof::Release()
	{
		m_depthRT.Release();
	}
	
	void CDof::Create(const SGraphicsConfig& config)
	{
		if (config.dofConfig.isEnable) {
			//DOFが有効。
			m_isEnable = config.dofConfig.isEnable;
			//DOFはメインレンダリングターゲットに書き込むときにMRTを使用して書き込むので、
			//メインと同じ幅と高さ。
			int w = Engine().GetFrameBufferWidth();
			int h = Engine().GetFrameBufferHeight();
			//16bit。
			m_depthRT.Create(w, h, 1, FMT_R16F, FMT_INVALID, MULTISAMPLE_NONE, 0);
			
			m_blurForward.Init(w, h, *Engine().GetMainRenderTarget().GetTexture());
			m_blurBack.Init(w, h, *Engine().GetMainRenderTarget().GetTexture());
			m_blurBack.SetBlurPower(2.0f);
			m_blurBack.SetUseWeights(CGaussianBlur::enUseWeight_8);
			m_blurForward.SetBlurPower(20.0f);
			m_blurForward.SetUseWeights(CGaussianBlur::enUseWeight_8);
			m_effect = EffectManager().LoadEffect("Assets/presetShader/dof.fx");
		}
	}
	
	void CDof::Render( CRenderContext& renderContext, CPostEffect* postEffect )
	{
		CPIXPerfTag tag(renderContext, L"CDof::Render");
		if (m_isEnable) {
			//被写体との距離から、こちらで計算する。
			static float CoC = 0.033f;			//許容錯乱円(単位はmm)
			float forwardDof = (CoC * m_F * m_pint * m_pint) / (m_focalLength * m_focalLength + CoC * m_F * m_pint);
			float backDof = (CoC * m_F * m_pint * m_pint) / (m_focalLength * m_focalLength - CoC * m_F * m_pint);
			//手前ボケ、奥ボケ、ピントをm単位に変更してGPUに送る
			float dofParam[] = {
				forwardDof / 1000.0f,
				backDof / 1000.0f,
				m_pint / 1000.0f
			};

			//ボケ画像を作成する。
			//奥ボケ
			{
				CPIXPerfTag tag(renderContext, L"CDof::Render(back blur)");
				m_blurBack.SetSrcTexture(*Engine().GetMainRenderTarget().GetTexture());
				m_blurBack.Draw(renderContext);
			}
			//手前ボケ
			{
				CPIXPerfTag tag(renderContext, L"CDof::Render(forward blur)");
				m_blurForward.SetSrcTexture(*Engine().GetMainRenderTarget().GetTexture());
				m_blurForward.Draw(renderContext);
			}
			
			//合成。
			{
				CPIXPerfTag tag(renderContext, L"CDof::Render(final)");
				m_effect->SetTechnique(renderContext, "Dof");
				m_effect->Begin(renderContext);
				m_effect->BeginPass(renderContext, 0);
				m_effect->SetValue(renderContext, "g_dofParam", dofParam, sizeof(dofParam));
				m_effect->SetTexture(renderContext, "g_scene", Engine().GetMainRenderTarget().GetTexture());
				m_effect->SetTexture(renderContext, "g_depthTexture", m_depthRT.GetTexture());
				m_effect->SetTexture(renderContext, "g_blurBack", m_blurBack.GetTexture());
				m_effect->SetTexture(renderContext, "g_blurForward", m_blurForward.GetTexture());

				float texSize[] = {
					s_cast<float>(m_depthRT.GetWidth()),
					s_cast<float>(m_depthRT.GetHeight()),
				};
				m_effect->SetValue(renderContext, "g_sceneTexSize", texSize, sizeof(texSize));
				m_effect->CommitChanges(renderContext);

				Engine().ToggleMainRenderTarget();
				renderContext.SetRenderTarget(0, &Engine().GetMainRenderTarget());

				postEffect->RenderFullScreen(renderContext);
				m_effect->EndPass(renderContext);
				m_effect->End(renderContext);
			}
			
		}
	}
}
