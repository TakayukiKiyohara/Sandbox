/*!
 *@brief	パーティクルのリソースクラス。
 */
#ifndef _TKPARTICLERESOURCES_H_
#define _TKPARTICLERESOURCES_H_

namespace tkEngine{
	/*!
	 *@brief	リソースクラス。
	 */
	class CParticleResources {
	private:
		CParticleResources();
		~CParticleResources();
	public:
		/*!
		 *@brief	テクスチャをロード。
		 *@param[in]	filePath		ファイルパス。
		 */
		CTexture* LoadTexture( const char* filePath );
		/*!
		*@brief	解放。
		*/
		void Release();
	private:
		friend class CEngine;
		std::map<int, CTexture*>	textureMap;
	};
}
#endif 
