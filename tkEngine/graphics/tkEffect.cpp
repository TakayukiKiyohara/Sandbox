/*!
 * @brief	�V�F�[�_�[�G�t�F�N�g�B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CEffect::CEffect(LPD3DXEFFECT effect) :
		m_pEffect(effect)
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CEffect::~CEffect()
	{
		if (m_pEffect) {
			m_pEffect->Release();
			m_pEffect = nullptr;
		}
	}
}