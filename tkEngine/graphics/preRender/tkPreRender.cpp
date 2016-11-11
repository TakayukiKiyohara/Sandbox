/*!
 * @brief	プリレンダリング。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkPreRender.h"

namespace tkEngine{
	CPreRender::CPreRender()
	{
	}
	CPreRender::~CPreRender()
	{
	}
	void CPreRender::Release()
	{
		m_idMap.Release();
		m_shadowMap.Release();
		m_reflectionMap.Release();
	}
	void CPreRender::Create( const SGraphicsConfig& config )
	{
		if (config.edgeRenderConfig.isEnable) {
			m_idMap.Create(config.edgeRenderConfig.idMapWidth, config.edgeRenderConfig.idMapHeight);
		}
		if (config.shadowRenderConfig.isEnable) {
			m_shadowMap.Create(
				config.shadowRenderConfig.shadowMapWidth,
				config.shadowRenderConfig.shadowMapHeight,
				config.shadowRenderConfig.isDisableSoftShadow,
				config.shadowRenderConfig.numShadowMap);
		}
		if (config.reflectionMapConfig.isEnable) {
			m_reflectionMap.Init(config.reflectionMapConfig.reflectionMapWidth, config.reflectionMapConfig.reflectionMapHeight);
		}
	}
}

