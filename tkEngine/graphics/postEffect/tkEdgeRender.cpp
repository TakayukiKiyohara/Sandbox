/*!
 * @brief	�G�b�W�`��B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkEdgeRender.h"
#include "tkEngine/graphics/preRender/tkIDMap.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	CEdgeRender::CEdgeRender() :
		m_isEnable(false)
	{
	}
	CEdgeRender::~CEdgeRender()
	{
	}
	void CEdgeRender::Create(const SGraphicsConfig& config)
	{
		if (config.edgeRenderConfig.isEnable) {
			m_isEnable = true;
			//�G�t�F�N�g�t�@�C�������[�h�B
			m_pEffect = CEngine::Instance().EffectManager().LoadEffect("Assets/presetshader/idMap.fx");
		}
	}
	void CEdgeRender::Render( CRenderContext& renderContext, CPostEffect* postEffect)
	{
		CPIXPerfTag tag(renderContext, L"CEdgeRender::Render");
		if (m_isEnable) {
			renderContext.SetRenderState(RS_ZENABLE, D3DZB_FALSE);
			//IDMap�̃e�N�X�`�����擾�B
			const CTexture* pIdMap = CEngine::Instance().IDMap().GetTexture();
			//�|�X�g�G�t�F�N�g�p�̃e�N�j�b�N��ݒ�B
			float idMapSize[] = {
				s_cast<float>(CEngine::Instance().IDMap().GetWitdh()),
				s_cast<float>(CEngine::Instance().IDMap().GetHeight()),
			};
			m_pEffect->SetTechnique(renderContext, "RenderEdge");
			m_pEffect->SetTexture(renderContext, "g_idMap", pIdMap);
			m_pEffect->SetValue(renderContext, "g_idMapSize", idMapSize, sizeof(idMapSize));
			m_pEffect->Begin(renderContext);
			m_pEffect->BeginPass(renderContext, 0);
			postEffect->RenderFullScreen(renderContext);
			m_pEffect->EndPass(renderContext);
			m_pEffect->End(renderContext);
			renderContext.SetRenderState(RS_ZENABLE, D3DZB_TRUE);
		}
	}
}
