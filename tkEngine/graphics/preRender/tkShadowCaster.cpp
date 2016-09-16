/*!
 * @brief	シャドウキャスター。
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkShadowCaster.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
////////////////////////////////////////////////////////////////
// シャドウキャスター(プリミティブ版)
////////////////////////////////////////////////////////////////
	CShadowCaster_Primitive::CShadowCaster_Primitive() :
		m_prim(nullptr)
	{
	}
	CShadowCaster_Primitive::~CShadowCaster_Primitive()
	{
	}
	void CShadowCaster_Primitive::Create(CPrimitive* prim)
	{
		m_prim = prim;
	}
	
	void CShadowCaster_Primitive::Render( CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP )
	{
		if(m_prim){
			CMatrix mWVP;
			mWVP.Mul(m_mWorld, mLVP);
			pEffect->BeginPass(renderContext, 0);
			pEffect->SetValue(renderContext, "g_mWVP", &mWVP, sizeof(mWVP));
			pEffect->CommitChanges(renderContext);
			renderContext.SetVertexDeclaration(m_prim->GetVertexDecl());
			renderContext.SetStreamSource(0, m_prim->GetVertexBuffer());
			renderContext.SetIndices(m_prim->GetIndexBuffer());
			renderContext.DrawIndexedPrimitive(m_prim);
			pEffect->EndPass(renderContext);
		}
	}
////////////////////////////////////////////////////////////////
// シャドウキャスター(スキンモデル版)
////////////////////////////////////////////////////////////////
	CShadowCaster_SkinModel::CShadowCaster_SkinModel() :
		m_skinModel(nullptr)
	{
	}
	CShadowCaster_SkinModel::~CShadowCaster_SkinModel()
	{
	}
	void CShadowCaster_SkinModel::Create(CSkinModel* model)
	{
		m_skinModel = model;
	}
	void CShadowCaster_SkinModel::Render(CRenderContext& renderContext, CEffect* pEffect, const CMatrix& mLVP)
	{
		if(m_skinModel){
			m_skinModel->DrawToShadowMap(renderContext, mLVP, CMatrix::Identity);
		}
	}
}