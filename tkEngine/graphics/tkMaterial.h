/*!
 * @brief	�}�e���A���B
 */

#ifndef _TKMATERIAL_H_
#define _TKMATERIAL_H_

#include "tkEngine/util/tkUtil.h"

namespace tkEngine{
	/*!
	 *@brief	�}�e���A���B
	 */
	class CMaterial{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CMaterial();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CMaterial();
		/*!
		 *@brief	�e�N�X�`����ݒ�B
		 *@param[in]	paramName		�p�����[�^���B
		 *@param[in]	texture			�e�N�X�`���B
		 */
		void SetTexture( const char* paramName, CTexture* texture )
		{
			int hash = CUtil::MakeHash(paramName);
			textureMap.insert(std::pair<int, CTexture*>(hash, texture));
		}
		/*!
		 *@brief	�}�e���A�������擾�B
		 */
		const char* GetMaterialName() const
		{
			return name.c_str();
		}
	private:
		CEffect*					effect;		//!<�G�t�F�N�g�B
		std::string					name;		//!<�}�e���A�����B
		std::map<int, CTexture*>	textureMap;	//!<�e�N�X�`���̃}�b�v�B
	};
}

#endif //_TKMATERIAL_H_