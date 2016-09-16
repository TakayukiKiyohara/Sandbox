/*!
 * @brief	�G�t�F�N�g�}�l�[�W���[
 */

#ifndef _TKEFFECTMANAGER_H_
#define _TKEFFECTMANAGER_H_

namespace tkEngine{
	class CEffect;
	/*!
	 * @brief	�G�t�F�N�g�}�l�[�W���B
	 */
	class CEffectManager : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CEffectManager();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CEffectManager();
		/*!
		 *@brief	�G�t�F�N�g�̃��[�h�B
		 *@details
		 * �ǂݍ��ݍς݂̃G�t�F�N�g�̏ꍇ�̓��[�h�͍s��ꂸ�A</br>
		 * �����̃G�t�F�N�g���Ԃ��Ă��܂��B
		 *@param[in]	filePath	���[�h����G�t�F�N�g�̃t�@�C���p�X�B
		 *@return	���[�h�����G�t�F�N�g�t�@�C���B
		 */
		CEffect* LoadEffect( const char* filePath );
		/*!
		 * @brief	�����[�X�B
		 */
		void Release();
	private:
		std::map<int, CEffect*>		m_effectDictinary;	//!<CEffect�̃C���X�^���X�̃f�B�N�V���i���B�t�@�C���p�X�̃n�b�V���l���L�[�Ɏ����ACEffect�̃C���X�^���X��l�Ɏ��B
	};
}

#endif /_TKEFFECTMANAGER_H_