/*!
 * @brief	テクスチャ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkTexture.h"

namespace tkEngine {
	/*!
	 * @brief	テクスチャをロード。
	 */
	void CTexture::Load(const char* fileName)
	{
		LPDIRECT3DDEVICE9 device = Engine().GetD3DDevice();
		if ( FAILED( D3DXCreateTextureFromFile(
			device,
			fileName,
			&m_tex))) {
			TK_ASSERT(false, "failed loadtexture");
		}
	}
}