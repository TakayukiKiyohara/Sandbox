/*!
 *@brief	�g�`�f�[�^�o���N�B
 */
#pragma once

#include <map>
namespace tkEngine{
	class CWaveFile;
	typedef std::shared_ptr<CWaveFile>	CWaveFilePtr;
	typedef std::map<unsigned int , CWaveFilePtr>	CWaveFilePtrMap;
	/*!
	 *@brief	�g�`�f�[�^�o���N�B
	 *@details
	 * ��x���[�h���ꂽ�g�`�f�[�^���o���N�ɓo�^���邱�Ƃ��o���܂��B
	 * �o�^���ꂽ�g�`�f�[�^�͍ēǂݍ��݂��s���K�v���Ȃ��A�o���N����ė��p���邱�Ƃ��o���܂��B
	 */
	class CWaveFileBank : Noncopyable{
	public:
		CWaveFileBank();
		~CWaveFileBank();
		/*!
		 *@brief	�g�`�f�[�^��o�^�B
		 *@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
		 *@param[in]	waveFile	�g�`�f�[�^�B
		 */
		void RegistWaveFile(int groupID, CWaveFilePtr waveFile);
		/*!
		*@brief	�����Ŏw�肵���t�@�C���p�X�̔g�`�f�[�^���o���N�ɓo�^����Ă��邩��������B
		*@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
		*@param[in]	filePath	�t�@�C���p�X�B
		*@return �g�`�f�[�^�B�o�^����Ă��Ȃ��ꍇ��NULL���Ԃ�B
		*/
		CWaveFilePtr FindWaveFile(int groupID, const char* filePath);
		/*!
		*@brief	�����Ŏw�肵���t�@�C���p�X�̔g�`�f�[�^���o���N�ɓo�^����Ă��邩��������B
		*@details 
		*  �������̂ق����������������B
		*@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
		*@param[in]	nameyKey	���O�L�[�B�B
		*@return �g�`�f�[�^�B�o�^����Ă��Ȃ��ꍇ��NULL���Ԃ�B
		*/
		CWaveFilePtr FindWaveFile(int groupID, const NameKey& nameKey);
		/*!
		*@brief	�O���[�v�P�ʂŉ���B
		*/
		void Release(int groupID);
		/*!
		*@brief	�S�ĉ���B
		*/
		void ReleaseAll()
		{
			for (int i = 0; i < MAX_GROUP; i++) {
				Release(i);
			}
		}
	private:
		static const int MAX_GROUP = 256;
		CWaveFilePtrMap	m_waveFileMap[MAX_GROUP];		//!<wave�t�@�C���̃��X�g�B
	};
}