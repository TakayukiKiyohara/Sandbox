/*!
 * @brief	マテリアル。
 */

#ifndef _TKMATERIAL_H_
#define _TKMATERIAL_H_

#include "tkEngine/util/tkUtil.h"

namespace tkEngine{
	/*!
	 *@brief	マテリアル。
	 */
	class CMaterial{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CMaterial();
		/*!
		 * @brief	デストラクタ。
		 */
		~CMaterial();
		/*!
		 *@brief	テクスチャを設定。
		 *@param[in]	paramName		パラメータ名。
		 *@param[in]	texture			テクスチャ。
		 */
		void SetTexture( const char* paramName, CTexture* texture )
		{
			int hash = CUtil::MakeHash(paramName);
			textureMap.insert(std::pair<int, CTexture*>(hash, texture));
		}
		/*!
		 *@brief	マテリアル名を取得。
		 */
		const char* GetMaterialName() const
		{
			return name.c_str();
		}
	private:
		CEffect*					effect;		//!<エフェクト。
		std::string					name;		//!<マテリアル名。
		std::map<int, CTexture*>	textureMap;	//!<テクスチャのマップ。
	};
}

#endif //_TKMATERIAL_H_