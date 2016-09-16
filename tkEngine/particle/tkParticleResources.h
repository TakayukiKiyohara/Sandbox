/*!
 *@brief	�p�[�e�B�N���̃��\�[�X�N���X�B
 */
#ifndef _TKPARTICLERESOURCES_H_
#define _TKPARTICLERESOURCES_H_

namespace tkEngine{
	/*!
	 *@brief	���\�[�X�N���X�B
	 */
	class CParticleResources {
	private:
		CParticleResources();
		~CParticleResources();
	public:
		/*!
		 *@brief	�e�N�X�`�������[�h�B
		 *@param[in]	filePath		�t�@�C���p�X�B
		 */
		CTexture* LoadTexture( const char* filePath );
		/*!
		*@brief	����B
		*/
		void Release();
	private:
		friend class CEngine;
		std::map<int, CTexture*>	textureMap;
	};
}
#endif 
