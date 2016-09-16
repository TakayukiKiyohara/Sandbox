/*!
 * @brief	4点ブラー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkBlur.h"
#include "tkEngine/shape/tkShapeVertex.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine {
	CBlur::CBlur() :
		m_srcTexture(nullptr),
		m_effect(nullptr)
	{

	}
	CBlur::~CBlur()
	{

	}

	void CBlur::Init(int w, int h, const CTexture& srcTexture)
	{
		m_srcTexture = &srcTexture;
		LPDIRECT3DTEXTURE9 tex = m_srcTexture->GetTextureDX();
		D3DSURFACE_DESC desc;
		tex->GetLevelDesc(0, &desc);
		m_srcTexWH[0] = w;
		m_srcTexWH[1] = h;
		int size[2][2]{
			{w >> 1, h},
			{ w >> 1, h >> 1},
		};
		for (int i = 0; i < 2; i++) {
			m_rt[i].Create(size[i][0], size[i][1], 1, (EFormat)desc.Format, FMT_INVALID, MULTISAMPLE_NONE, 0);
		}
		
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
		m_prim.Create(
			CPrimitive::eTriangleStrip,
			4,
			sizeof(SShapeVertex_PT),
			scShapeVertex_PT_Element,
			vertex,
			4,
			eIndexFormat16,
			index
			);
		m_effect = EffectManager().LoadEffect("Assets/presetShader/TransformedPrim.fx");

	}
	void CBlur::Draw(CRenderContext& renderContext)
	{
		CRenderTarget* rtBackup = renderContext.GetRenderTarget(0);

		//Xブラー。
		{
			renderContext.SetRenderTarget(0, &m_rt[0]);
			float size[2] = {
				s_cast<float>(m_srcTexWH[0]),
				s_cast<float>(m_srcTexWH[1])
			};
			m_effect->SetTechnique(renderContext, "TransformedPrimBlurX");
			m_effect->Begin(renderContext);
			m_effect->BeginPass(renderContext, 0);
			m_effect->SetTexture(renderContext, "g_tex", m_srcTexture);
			m_effect->SetValue(renderContext, "g_texSize", size, sizeof(size));
			m_effect->CommitChanges(renderContext);
			renderContext.SetVertexDeclaration(m_prim.GetVertexDecl());
			renderContext.SetStreamSource(0, m_prim.GetVertexBuffer());
			renderContext.SetIndices(m_prim.GetIndexBuffer());
			renderContext.DrawIndexedPrimitive(&m_prim);

			m_effect->EndPass(renderContext);
			m_effect->End(renderContext);
		}
		//Yブラー。
		{
			renderContext.SetRenderTarget(0, &m_rt[1]);

			float size[2] = {
				s_cast<float>(m_rt[0].GetWidth()),
				s_cast<float>(m_rt[0].GetHeight())
			};
			m_effect->SetTechnique(renderContext, "TransformedPrimBlurY");
			m_effect->Begin(renderContext);
			m_effect->BeginPass(renderContext, 0);
			m_effect->SetTexture(renderContext, "g_tex", m_rt[0].GetTexture());
			m_effect->SetValue(renderContext, "g_texSize", size, sizeof(size));
			m_effect->CommitChanges(renderContext);
			renderContext.SetVertexDeclaration(m_prim.GetVertexDecl());
			renderContext.SetStreamSource(0, m_prim.GetVertexBuffer());
			renderContext.SetIndices(m_prim.GetIndexBuffer());
			renderContext.DrawIndexedPrimitive(&m_prim);

			m_effect->EndPass(renderContext);
			m_effect->End(renderContext);
		}
		renderContext.SetRenderTarget(0, rtBackup);

	}
}