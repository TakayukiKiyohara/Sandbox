/*!
 * @brief	wave�t�@�C��
 */

#pragma once

#include <thread>

namespace tkEngine{
	/*!
	 *@brief	wave�t�@�C�����������߂̃N���X�B
	 *@details
	 * wave�t�@�C���̓����ǂݍ��݁A�񓯊��ǂݍ��݂��T�|�[�g���܂��B</br>
	 * CWaveFile::Read�͓����ǂݍ��݁ACWaveFile::ReadAsync�͔񓯊��ǂݍ��݂��s���܂��B</br>
	 * ReadAsync���g�p�����ꍇ��CWaveFile::IsReadEnd���g�p���ē������Ƃ��ĉ������B</br>
	 */
	class CWaveFile {
	public:
		CWaveFile();
		~CWaveFile();
		/*!
		 * @brief	wave�t�@�C�����I�[�v���B
		 *@param[in]	fileName		�t�@�C�����B
		 */
		void Open( const char* fileName );
		/*!
		* @brief	�g�`�f�[�^��ǂݍ��݁B
		*@param[out]	pBuffer		�g�`�f�[�^���������ރo�b�t�@�B
		*@param[in]		sizeToRead	�ǂݍ��ރf�[�^�T�C�Y�B
		*/
		void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
		/*!
		* @brief	�g�`�f�[�^��񓯊��ǂݍ��݁B
		*@param[out]	pBuffer		�g�`�f�[�^���������ރo�b�t�@�B
		*@param[in]		sizeToRead	�ǂݍ��ރf�[�^�T�C�Y�B
		*/
		void ReadAsync( char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
		/*!
		* @brief	�ǂݍ��ݏI������B
		*/
		bool IsReadEnd() const
		{
			return m_isReadEnd;
		}
		/*!
		* @brief	�g�`�f�[�^�̃V�[�N�ʒu�����Z�b�g�B
		*/
		void ResetFile();
		/*!
		 * @brief	�����[�X�B
		 */
		void Release();
		/*!
		* @brief	�t�@�C���T�C�Y���擾�B
		*/
		DWORD   GetSize() const
		{
			return m_dwSize;
		}
		/*!
		* @brief	�t�@�C���t�H�[�}�b�g���擾�B
		*/
		WAVEFORMATEX* GetFormat() const
		{
			return m_pwfx;
		};
		/*!
		* @brief	�t�@�C���p�X�̃n�b�V���l���擾�B
		*/
		unsigned int GetFilePathHash() const
		{
			return m_filePathHash;
		}
	private:
		HMMIO			m_hmmio = NULL;	//Wave�t�@�C���n���h���B
		WAVEFORMATEX* 	m_pwfx = NULL;  //wave�t�@�C���̃t�H�[�}�b�g��`�B
		MMCKINFO 		m_ckRiff;      // Use in opening a WAVE file
		DWORD			m_dwSize = 0;      // The size of the wave file
		MMCKINFO		m_ck;          // Multimedia RIFF chunk
		BYTE*			m_pbData = NULL;
		BYTE*			m_pbDataCur = NULL;
		ULONG			m_ulDataSize = 0;
		bool			m_isReadEnd = true;	//�ǂݍ��ݏI���t���O�B
		std::thread		m_readAsyncThread;	//�񓯊��ǂݍ��݃X���b�h�B
		bool			m_isInvokeReadAsyncThread = false;
		std::string		m_filePath;			//�t�@�C���p�X�B
		unsigned int	m_filePathHash = 0;		//�t�@�C���p�X�̃n�b�V���R�[�h�B
	};
}