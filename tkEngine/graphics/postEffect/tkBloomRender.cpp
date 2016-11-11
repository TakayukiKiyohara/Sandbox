/*!
 * @brief	�u���[��
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkBloomRender.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkRenderTarget.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CBloomRender::CBloomRender()
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CBloomRender::~CBloomRender()
	{
	}
	void CBloomRender::UpdateWeight(float dispersion)
	{
		float total = 0;
		for (int i = 0; i<NUM_WEIGHTS; i++) {
			m_weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
			total += 2.0f*m_weights[i];
			
		}
		// �K�i��
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_weights[i] /= total;
		}
	}
	/*!
	 * @brief	�`��B
	 */
	void CBloomRender::Render( CRenderContext& renderContext, CPostEffect* postEffect )
	{
		CPIXPerfTag tag(renderContext, L"CBloomRender::Render");
		if (m_isEnable) {
			UpdateWeight(25.0f);
			CRenderTarget* rt = renderContext.GetRenderTarget(0);
			
			//�P�x���o
			{
				renderContext.SetRenderTarget(0, &m_luminanceRenderTarget);
				renderContext.Clear(0, nullptr, D3DCLEAR_TARGET , 0, 1.0f, 0);
				m_pEffect->SetTechnique(renderContext, "SamplingLuminance");
				m_pEffect->Begin(renderContext);
				m_pEffect->BeginPass(renderContext, 0);
				m_pEffect->SetTexture(renderContext, "g_scene", rt->GetTexture());
				m_pEffect->CommitChanges(renderContext);
				postEffect->RenderFullScreen(renderContext);

				m_pEffect->EndPass(renderContext);
				m_pEffect->End(renderContext);
				
			}
			//XBlur
			{
				renderContext.SetRenderTarget(0, &m_downSamplingRenderTarget[0]);
				renderContext.Clear(0, nullptr, D3DCLEAR_TARGET , 0, 1.0f, 0);
				m_pEffect->SetTechnique(renderContext, "XBlur");
				m_pEffect->Begin(renderContext);
				m_pEffect->BeginPass(renderContext, 0);
				float size[2] = {
					s_cast<float>(m_luminanceRenderTarget.GetWidth()),
					s_cast<float>(m_luminanceRenderTarget.GetHeight())
				};
				float offset[] = {
					16.0f / s_cast<float>(m_luminanceRenderTarget.GetWidth()),
					0.0f
				};
				m_pEffect->SetValue(renderContext, "g_luminanceTexSize", size, sizeof(size));
				m_pEffect->SetValue(renderContext, "g_offset", offset, sizeof(size));
				m_pEffect->SetValue(renderContext, "g_weight", m_weights, sizeof(m_weights));
				
				m_pEffect->SetTexture(renderContext, "g_blur", m_luminanceRenderTarget.GetTexture());
				m_pEffect->CommitChanges(renderContext);
				postEffect->RenderFullScreen(renderContext);

				m_pEffect->EndPass(renderContext);
				m_pEffect->End(renderContext);

			}
			//YBlur
			{
				renderContext.SetRenderTarget(0, &m_downSamplingRenderTarget[1]);
				renderContext.Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);
				m_pEffect->SetTechnique(renderContext, "YBlur");
				m_pEffect->Begin(renderContext);
				m_pEffect->BeginPass(renderContext, 0);
				float size[2] = {
					s_cast<float>(m_downSamplingRenderTarget[0].GetWidth()),
					s_cast<float>(m_downSamplingRenderTarget[0].GetHeight())
				};
				float offset[] = {
					0.0f,
					16.0f / s_cast<float>(m_downSamplingRenderTarget[0].GetHeight()),
				};
				m_pEffect->SetValue(renderContext, "g_luminanceTexSize", size, sizeof(size));
				m_pEffect->SetValue(renderContext, "g_offset", offset, sizeof(offset));
				m_pEffect->SetValue(renderContext, "g_weight", m_weights, sizeof(m_weights));

				m_pEffect->SetTexture(renderContext, "g_blur", m_downSamplingRenderTarget[0].GetTexture());
				m_pEffect->CommitChanges(renderContext);
				postEffect->RenderFullScreen(renderContext);

				m_pEffect->EndPass(renderContext);
				m_pEffect->End(renderContext);
			}

			//XBlur2
			{
				renderContext.SetRenderTarget(0, &m_downSamplingRenderTarget[2]);
				renderContext.Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);
				m_pEffect->SetTechnique(renderContext, "XBlur");
				m_pEffect->Begin(renderContext);
				m_pEffect->BeginPass(renderContext, 0);
				float size[2] = {
					s_cast<float>(m_downSamplingRenderTarget[1].GetWidth()),
					s_cast<float>(m_downSamplingRenderTarget[1].GetHeight())
				};
				float offset[] = {
					16.0f / s_cast<float>(m_downSamplingRenderTarget[1].GetWidth()),
					0.0f
				};
				m_pEffect->SetValue(renderContext, "g_luminanceTexSize", size, sizeof(size));
				m_pEffect->SetValue(renderContext, "g_offset", offset, sizeof(offset));
				m_pEffect->SetValue(renderContext, "g_weight", m_weights, sizeof(m_weights));

				m_pEffect->SetTexture(renderContext, "g_blur", m_downSamplingRenderTarget[1].GetTexture());
				m_pEffect->CommitChanges(renderContext);
				postEffect->RenderFullScreen(renderContext);

				m_pEffect->EndPass(renderContext);
				m_pEffect->End(renderContext);

			}
			//YBlur2
			{
				renderContext.SetRenderTarget(0, &m_downSamplingRenderTarget[3]);
				renderContext.Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);
				m_pEffect->SetTechnique(renderContext, "YBlur");
				m_pEffect->Begin(renderContext);
				m_pEffect->BeginPass(renderContext, 0);
				float size[2] = {
					s_cast<float>(m_downSamplingRenderTarget[2].GetWidth()),
					s_cast<float>(m_downSamplingRenderTarget[2].GetHeight())
				};
				float offset[] = {
					0.0f,
					16.0f / s_cast<float>(m_downSamplingRenderTarget[2].GetHeight()),
				};
				m_pEffect->SetValue(renderContext, "g_luminanceTexSize", size, sizeof(size));
				m_pEffect->SetValue(renderContext, "g_offset", offset, sizeof(offset));
				m_pEffect->SetValue(renderContext, "g_weight", m_weights, sizeof(m_weights));

				m_pEffect->SetTexture(renderContext, "g_blur", m_downSamplingRenderTarget[2].GetTexture());
				m_pEffect->CommitChanges(renderContext);
				postEffect->RenderFullScreen(renderContext);

				m_pEffect->EndPass(renderContext);
				m_pEffect->End(renderContext);
			}

			{
				float offset[] = {
					0.5f / m_downSamplingRenderTarget[3].GetWidth() ,
					0.5f / m_downSamplingRenderTarget[3].GetHeight()
				};
				//�߂��B
				renderContext.SetRenderTarget(0, rt);
				//���Z�����B
				renderContext.SetRenderState(RS_ALPHABLENDENABLE, TRUE);
				renderContext.SetRenderState(RS_SRCBLEND, D3DBLEND_ONE);
				renderContext.SetRenderState(RS_DESTBLEND, D3DBLEND_ONE);
				m_pEffect->SetTechnique(renderContext, "Final");
				m_pEffect->Begin(renderContext);
				m_pEffect->BeginPass(renderContext, 0);
				m_pEffect->SetTexture(renderContext, "g_blur", m_downSamplingRenderTarget[3].GetTexture());
				m_pEffect->SetValue(renderContext, "g_offset", offset, sizeof(offset));
				m_pEffect->CommitChanges(renderContext);
				postEffect->RenderFullScreen(renderContext);

				m_pEffect->EndPass(renderContext);
				m_pEffect->End(renderContext);

				renderContext.SetRenderState(RS_ALPHABLENDENABLE, FALSE);
				renderContext.SetRenderState(RS_SRCBLEND, D3DBLEND_SRCALPHA);
				renderContext.SetRenderState(RS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
		}

	}
	/*!
	* @brief	�J���B
	*/
	void CBloomRender::Release()
	{
		m_luminanceRenderTarget.Release();
		for (int i = 0; i < NUM_DOWN_SAMPLING_RT; i++) {
			m_downSamplingRenderTarget[i].Release();
		}
	}
	/*!
	 * @brief	�쐬�B
	 */
	void CBloomRender::Create( const SGraphicsConfig& config )
	{
		if (config.bloomConfig.isEnable) {
			int w = CEngine::Instance().GetFrameBufferWidth();
			int h = CEngine::Instance().GetFrameBufferHeight();
			//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬�B
			m_luminanceRenderTarget.Create(w, h, 1, FMT_A16B16G16R16F, FMT_INVALID, MULTISAMPLE_NONE, 0);
			//�_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g���쐬�B
			m_downSamplingRenderTarget[0].Create(w >> 1, h, 1, FMT_A16B16G16R16F, FMT_INVALID, MULTISAMPLE_NONE, 0);			//���u���[�p�B
			m_downSamplingRenderTarget[1].Create(w >> 1, h >> 1, 1, FMT_A16B16G16R16F, FMT_INVALID, MULTISAMPLE_NONE, 0);	//�c�u���[�p�B
			m_downSamplingRenderTarget[2].Create(w >> 2, h, 1, FMT_A16B16G16R16F, FMT_INVALID, MULTISAMPLE_NONE, 0);			//���u���[�p�B
			m_downSamplingRenderTarget[3].Create(w >> 2, h >> 2, 1, FMT_A16B16G16R16F, FMT_INVALID, MULTISAMPLE_NONE, 0);	//�c�u���[�p�B
			m_pEffect = CEngine::Instance().EffectManager().LoadEffect("Assets/presetShader/bloom.fx");
			m_isEnable = true;
		}
	}
}
