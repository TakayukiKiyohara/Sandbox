/*!4
 * @brief	シャドウマップ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkShadowMap.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/preRender/tkShadowCaster.h"
#include "tkEngine/graphics/tkCamera.h"



namespace tkEngine{
	namespace {
		const float shadowAreaTable[CShadowMap::MAX_SHADOW_MAP][2] =
		{
			{ 20.0f, 20.0f },
			{ 45.0f, 45.0f },
			{ 120.0f, 120.0f },
		};
	}
	CShadowMap::CShadowMap() :
		m_isEnable(false),
		m_pShadowMapEffect(nullptr),
		m_near(1.0f),
		m_far(100.0f),
		m_lvMatrix(CMatrix::Identity),
		m_accpect(1.0f),
		m_camera(nullptr),
		m_calcLightViewFunc(enCalcLightViewFunc_PositionTarget),
		m_lightTarget(CVector3::Zero),
		m_isDisableVSM(false),
		m_numShadowMap(1)
	{
		m_lightPosition.Set(0.0f, 3.0f, 0.0f);
		m_lightDirection.Set(0.0f, -1.0f, 0.0f);
		
		for (int i = 0; i < MAX_SHADOW_MAP; i++) {
			m_shadowAreaW[i] = shadowAreaTable[i][0];
			m_shadowAreaH[i] = shadowAreaTable[i][1];
		}
	}
	CShadowMap::~CShadowMap()
	{
		Release();
	}
	void CShadowMap::Create( int w, int h, bool isDisableSoftShadow, int numShadowMap)
	{
		TK_ASSERT( numShadowMap <= MAX_SHADOW_MAP, "numShadowMap is invalid");
		Release();
		
		
		m_near = 1.0f;
		m_far = 100.0f;
		m_numShadowMap = numShadowMap;
		m_isEnable = true;
		int j = 0;
		for (int i = MAX_SHADOW_MAP - m_numShadowMap; i < MAX_SHADOW_MAP; i++, j++) {
			m_shadowAreaW[j] = shadowAreaTable[i][0];
			m_shadowAreaH[j] = shadowAreaTable[i][1];
		}
		m_pShadowMapEffect = CEngine::EffectManager().LoadEffect("Assets/presetshader/shadowMap.fx");
		int wh[MAX_SHADOW_MAP][2] = {
			{w, h},
			{ w >> 1, h >> 1},
			{ w >> 1, h >> 1},
		};
		for (int i = 0; i < m_numShadowMap; i++) {
			m_shadowMapRT[i].Create(wh[i][0], wh[i][1], 1, FMT_G16R16F, FMT_D16, MULTISAMPLE_NONE, 0);
			m_accpect = s_cast<float>(w) / s_cast<float>(h);
#ifdef USE_ORTHO_PROJECTION
			m_projectionMatrix.MakeOrthoProjectionMatrix(
				m_shadowAreaW[i] * m_accpect, 
				m_shadowAreaH[i], 
				m_near, 
				m_far
			);
#else
			m_projectionMatrix.MakeProjectionMatrix(
				CMath::DegToRad(60.0f),
				m_accpect,
				m_near,
				m_far
				);
#endif
		}
		
		m_isDisableVSM = isDisableSoftShadow;
		if (!m_isDisableVSM) {
			//VSMを行う。
			for (int i = 0; i < m_numShadowMap; i++) {
				m_blur[i].Init(w, h, *m_shadowMapRT[i].GetTexture());
			}
		}
	}
	void CShadowMap::Release()
	{
		if (m_shadowMapRT) {
			for (int i = 0; i < m_numShadowMap; i++) {
				m_shadowMapRT[i].Release();
			}
		}
	}
	void CShadowMap::Entry( IShadowCaster* caster )
	{
		m_shadowCaster.push_back(caster);
	}
	/*!
	* @brief	更新。
	*/
	void CShadowMap::Update()
	{
		if (m_isEnable) {
			if (m_calcLightViewFunc == enCalcLightViewFunc_PositionTarget) {
				//ライトの位置と注視点で計算。
				m_lightDirection.Subtract(m_lightTarget, m_lightPosition);
				m_lightDirection.Normalize();

			}
			//ライトビュープロジェクション行列を作成。
			CVector3 lightUp;
			float t = fabsf(m_lightDirection.Dot(CVector3::AxisY));
			if (fabsf((t - 1.0f)) < 0.00001f) {
				//ライトの方向がほぼY軸と並行。
				lightUp = CVector3::AxisX;
			}
			else {
				lightUp = CVector3::AxisY;
			}
			//ライトからみたビュー行列を作成。
			CVector3 target;
			target.Add(m_lightPosition, m_lightDirection);
			m_lvMatrix.MakeLookAt(m_lightPosition, target, lightUp); 
			for (int i = 0; i < m_numShadowMap; i++) {
#ifdef USE_ORTHO_PROJECTION
				m_projectionMatrix.MakeOrthoProjectionMatrix(
					m_shadowAreaW[i] * m_accpect, 
					m_shadowAreaH[i], 
					m_near, 
					m_far
				);
#else
				m_projectionMatrix.MakeProjectionMatrix(
					CMath::DegToRad(60.0f),
					m_accpect,
					m_near,
					m_far
					);
#endif
				m_LVPMatrix[i].Mul(m_lvMatrix, m_projectionMatrix);
				m_shadowRecieverParam.mLVP[i] = m_LVPMatrix[i];
			}
			m_shadowRecieverParam.vsmFlag_numShadowMap[0] = m_isDisableVSM ? 0 : 1;
			m_shadowRecieverParam.vsmFlag_numShadowMap[1] = m_numShadowMap;
		}
	}
	void CShadowMap::RenderToShadowMap( CRenderContext& renderContext )
	{
		CPIXPerfTag tag(renderContext, L"CShadowMap::RenderToShadowMap");
		if (m_isEnable) {

			CRenderTarget* pRTBackup = renderContext.GetRenderTarget(0);
			for (int i = 0; i < m_numShadowMap; i++) {
				renderContext.SetRenderTarget(0, &m_shadowMapRT[i]);
				renderContext.Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
				renderContext.SetRenderState(RS_CULLMODE, CULL_NONE);
				for (auto caster : m_shadowCaster) {
					caster->Render(renderContext, m_pShadowMapEffect, m_LVPMatrix[i]);
				}
				renderContext.SetRenderState(RS_CULLMODE, CULL_CCW);
				//ブラーをかけて深度マップの平均値を求める。
				if (!m_isDisableVSM) {
					m_blur[i].Draw(renderContext);
				}
			}
			renderContext.SetRenderTarget(0, pRTBackup);
			m_shadowCaster.clear();	
		}
	}
}