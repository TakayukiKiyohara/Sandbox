/*!
 * @brief	�e�N�X�`���B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkTexture.h"

namespace tkEngine {
	/*!
	 * @brief	�e�N�X�`�������[�h�B
	 */
	bool CTexture::Load(const char* fileName)
	{
		LPDIRECT3DDEVICE9 device = Engine().GetD3DDevice();
		if ( FAILED( D3DXCreateTextureFromFile(
			device,
			fileName,
			&m_tex))) {
			TK_LOG("FailedTextureLoad");
			return false;
		}
		ComputeTexSize();
		return true;
	}
}