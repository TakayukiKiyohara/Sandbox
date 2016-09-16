/*!
 * @brief	シェーダーエフェクト。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CEffect::CEffect(LPD3DXEFFECT effect) :
		m_pEffect(effect)
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CEffect::~CEffect()
	{
		if (m_pEffect) {
			m_pEffect->Release();
			m_pEffect = nullptr;
		}
	}
}