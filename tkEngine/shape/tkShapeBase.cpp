/*!
 *@brief	形状の基本クラス。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/shape/tkShapeBase.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkLight.h"

namespace tkEngine{
	CLight	CShapeBase::m_defaultLight;
	bool CShapeBase::m_isInitDefaultLight = false;
	CShapeBase::CShapeBase() :
		m_isCreatePrimitive(false),
		m_pPrimitive(nullptr),
		m_position(CVector3::Zero),
		m_worldMatrix(CMatrix::Identity),
		m_rotationMatrix(CMatrix::Identity),
		m_pEffect(nullptr)
	{
		if (!m_isInitDefaultLight) {
			//デフォルトライトの初期化。
			m_defaultLight.SetAmbinetLight(CVector3(1.0f, 1.0f, 1.0f));
			m_isInitDefaultLight = false;
		}
	}
	CShapeBase::~CShapeBase()
	{
		Release();
	}
	void CShapeBase::Release()
	{
		if(!m_isCreatePrimitive){
			delete m_pPrimitive;
			m_pPrimitive = nullptr;
			m_isCreatePrimitive = false;
		}
	}
	void CShapeBase::UpdateWorldMatrix()
	{
		m_rotationMatrix.MakeRotationFromQuaternion(m_rotation);
		CMatrix mTrans;
		mTrans.MakeTranslation(m_position);
		m_worldMatrix.Mul(m_rotationMatrix, mTrans );
	}
	void CShapeBase::Render(CRenderContext& renderContext)
	{
		if (m_pPrimitive == nullptr) {
			return;
		}
		renderContext.SetVertexDeclaration(m_pPrimitive->GetVertexDecl());
		renderContext.SetStreamSource( 0, m_pPrimitive->GetVertexBuffer() );
		renderContext.SetIndices( m_pPrimitive->GetIndexBuffer() );
		renderContext.DrawIndexedPrimitive(	m_pPrimitive);
	}
	void CShapeBase::CreateEffect(bool hasNormal)
	{
		
		if (hasNormal) {
			m_pEffect = tkEngine::CEngine::Instance().EffectManager().LoadEffect("Assets/presetShader/ColorNormalPrim.fx");
		}
		else {
			m_pEffect = tkEngine::CEngine::Instance().EffectManager().LoadEffect("Assets/presetShader/ColorNormal.fx");
		}
	}
	void CShapeBase::RenderLight(
		CRenderContext& renderContext,
		const CMatrix& viewProjectionMatrix,
		const CLight& light,
		bool isIluminance,
		bool isReceiveShadow
	)
	{
		CMatrix mWVP;
		mWVP.Mul(m_worldMatrix, viewProjectionMatrix);
		RenderLightWVP(renderContext, mWVP, light, isIluminance, isReceiveShadow);
	}
	void CShapeBase::RenderLightWVP(
		CRenderContext& renderContext,
		const CMatrix& mWVP,
		const CLight& light,
		bool isIluminance,
		bool isReceiveShadow,
		const CMatrix* pmWorldMatrix
	)
	{
		if (isIluminance) {
			if(isReceiveShadow){
				m_pEffect->SetTechnique(renderContext, "ColorNormalShadowPrimIuminance");
			}
			else {
				m_pEffect->SetTechnique(renderContext, "ColorNormalPrimIuminance");
			}
		}
		else {
			if (isReceiveShadow) {
				m_pEffect->SetTechnique(renderContext, "ColorNormalShadowPrim");
			}
			else {
				m_pEffect->SetTechnique(renderContext, "ColorNormalPrim");
			}
		}
		m_pEffect->Begin(renderContext);
		m_pEffect->BeginPass(renderContext, 0);
		m_pEffect->SetValue(renderContext, "g_mWVP", &mWVP, sizeof(mWVP));
		m_pEffect->SetValue(renderContext, "g_worldRotationMatrix", &m_rotationMatrix, sizeof(m_rotationMatrix));
		if (isReceiveShadow) {
			CShadowMap& shadowMap = CEngine::Instance().ShadowMap();
			const CMatrix& mLVP = shadowMap.GetLVPMatrix();
			float farNear[] = {
				shadowMap.GetFar(),
				shadowMap.GetNear()
			};
			m_pEffect->SetValue(renderContext, "g_mLVP", &mLVP, sizeof(mLVP));
			m_pEffect->SetValue(renderContext, "g_farNear", farNear, sizeof(farNear));
			m_pEffect->SetTexture(renderContext, "g_shadowMap_0", shadowMap.GetTexture(0));
			if (pmWorldMatrix) {
				m_pEffect->SetValue(renderContext, "g_mWorld", pmWorldMatrix, sizeof(*pmWorldMatrix));
			}
			else {
				m_pEffect->SetValue(renderContext, "g_mWorld", &m_worldMatrix, sizeof(m_worldMatrix));
			}
		}
		m_pEffect->SetValue(
			renderContext,
			"g_light",
			&light,
			sizeof(CLight)
			);
		m_pEffect->CommitChanges(renderContext);
		Render(renderContext);

		m_pEffect->EndPass(renderContext);
		m_pEffect->End(renderContext);
	}
}
