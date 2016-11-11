/*!
 * @brief	��ʊE�[�x�B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkDOF.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CDof::CDof() :
		m_isEnable(false),
		m_pint(3000.0f),
		m_focalLength(24.0f),
		m_F(5.6f)
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
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
			//DOF���L���B
			m_isEnable = config.dofConfig.isEnable;
			//DOF�̓��C�������_�����O�^�[�Q�b�g�ɏ������ނƂ���MRT���g�p���ď������ނ̂ŁA
			//���C���Ɠ������ƍ����B
			int w = Engine().GetFrameBufferWidth();
			int h = Engine().GetFrameBufferHeight();
			//16bit�B
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
			//��ʑ̂Ƃ̋�������A������Ōv�Z����B
			static float CoC = 0.033f;			//���e�����~(�P�ʂ�mm)
			float forwardDof = (CoC * m_F * m_pint * m_pint) / (m_focalLength * m_focalLength + CoC * m_F * m_pint);
			float backDof = (CoC * m_F * m_pint * m_pint) / (m_focalLength * m_focalLength - CoC * m_F * m_pint);
			//��O�{�P�A���{�P�A�s���g��m�P�ʂɕύX����GPU�ɑ���
			float dofParam[] = {
				forwardDof / 1000.0f,
				backDof / 1000.0f,
				m_pint / 1000.0f
			};

			//�{�P�摜���쐬����B
			//���{�P
			{
				CPIXPerfTag tag(renderContext, L"CDof::Render(back blur)");
				m_blurBack.SetSrcTexture(*Engine().GetMainRenderTarget().GetTexture());
				m_blurBack.Draw(renderContext);
			}
			//��O�{�P
			{
				CPIXPerfTag tag(renderContext, L"CDof::Render(forward blur)");
				m_blurForward.SetSrcTexture(*Engine().GetMainRenderTarget().GetTexture());
				m_blurForward.Draw(renderContext);
			}
			
			//�����B
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
