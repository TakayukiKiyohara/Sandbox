/*!
 * @brief	ポストエフェクト
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/postEffect/tkPostEffect.h"
#include "tkEngine/shape/tkShapeVertex.h"
namespace tkEngine{
	CPostEffect::CPostEffect()
	{
	}
	CPostEffect::~CPostEffect()
	{
	}
	/*!
	* @brief	開放。
	*/
	void CPostEffect::Release()
	{
		m_bloomRender.Release();
		m_dof.Release();
		m_fxaa.Release();
	}
	void CPostEffect::Create( const SGraphicsConfig& config )
	{
		if(config.edgeRenderConfig.isEnable){
			m_edgeRender.Create(config );
		}
		if (config.bloomConfig.isEnable) {
			m_bloomRender.Create(config);
		}
		m_dof.Create(config);
		m_fxaa.Init(config);
		m_motionBlur.Create(config);
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
		m_fullscreenRenderPrim.Create(
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
	void CPostEffect::Render(CRenderContext& renderContext)
	{
		renderContext.SetRenderTarget(1, NULL);
		renderContext.SetRenderTarget(2, NULL);
		m_edgeRender.Render(renderContext, this);
		m_motionBlur.Render(renderContext, this);
		m_dof.Render(renderContext, this);
		m_bloomRender.Render(renderContext, this);
		m_fxaa.Render(renderContext, this);
		
	}
	/*!
	* @brief	フルスクリーン描画。
	*/
	void CPostEffect::RenderFullScreen(CRenderContext& renderContext)
	{
		renderContext.SetVertexDeclaration(m_fullscreenRenderPrim.GetVertexDecl());
		renderContext.SetStreamSource(0, m_fullscreenRenderPrim.GetVertexBuffer());
		renderContext.SetIndices(m_fullscreenRenderPrim.GetIndexBuffer());
		renderContext.DrawIndexedPrimitive(&m_fullscreenRenderPrim);
	}
}