/*!
 * @brief	�}�e���A��
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelMaterial.h"
#include "tkEngine/graphics/tkEffect.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CSkinModelMaterial::CSkinModelMaterial()
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CSkinModelMaterial::~CSkinModelMaterial()
	{
	}
	/*!
	*@brief	�}�e���A������GPU�ɓ]���B
	*@param[in]		effect		�G�t�F�N�g�B
	*/
	void CSkinModelMaterial::SendMaterialParamToGPUImmidiate(ID3DXEffect* effect)
	{
		//�}�e���A����]���B
		for (auto& p : textureMap) {
			effect->SetTexture(p.second.paramName.c_str(), p.second.param->GetTextureDX());
		}
	}
}