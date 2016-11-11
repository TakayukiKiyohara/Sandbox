/*!
 * @brief	マテリアル
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CSkinModelMaterial::CSkinModelMaterial()
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CSkinModelMaterial::~CSkinModelMaterial()
	{
	}
	/*!
	*@brief	マテリアル情報をGPUに転送。
	*@param[in]		effect		エフェクト。
	*/
	void CSkinModelMaterial::SendMaterialParamToGPUImmidiate(ID3DXEffect* effect)
	{
		//マテリアルを転送。
		for (auto& p : textureMap) {
			effect->SetTexture(p.second.paramName.c_str(), p.second.param->GetTextureDX());
		}
	}
}