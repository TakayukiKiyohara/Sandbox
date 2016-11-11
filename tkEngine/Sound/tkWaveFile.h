/*!
 * @brief	waveファイル
 */

#pragma once

#include <thread>

namespace tkEngine{
	/*!
	 *@brief	waveファイルを扱うためのクラス。
	 *@details
	 * waveファイルの同期読み込み、非同期読み込みをサポートします。</br>
	 * CWaveFile::Readは同期読み込み、CWaveFile::ReadAsyncは非同期読み込みが行われます。</br>
	 * ReadAsyncを使用した場合はCWaveFile::IsReadEndを使用して同期をとって下さい。</br>
	 */
	class CWaveFile {
	public:
		CWaveFile();
		~CWaveFile();
		/*!
		 * @brief	waveファイルをオープン。
		 *@param[in]	fileName		ファイル名。
		 */
		void Open( const char* fileName );
		/*!
		* @brief	波形データを読み込み。
		*@param[out]	pBuffer		波形データを書き込むバッファ。
		*@param[in]		sizeToRead	読み込むデータサイズ。
		*/
		void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
		/*!
		* @brief	波形データを非同期読み込み。
		*@param[out]	pBuffer		波形データを書き込むバッファ。
		*@param[in]		sizeToRead	読み込むデータサイズ。
		*/
		void ReadAsync( char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
		/*!
		* @brief	読み込み終了判定。
		*/
		bool IsReadEnd() const
		{
			return m_isReadEnd;
		}
		/*!
		* @brief	波形データのシーク位置をリセット。
		*/
		void ResetFile();
		/*!
		 * @brief	リリース。
		 */
		void Release();
		/*!
		* @brief	ファイルサイズを取得。
		*/
		DWORD   GetSize() const
		{
			return m_dwSize;
		}
		/*!
		* @brief	ファイルフォーマットを取得。
		*/
		WAVEFORMATEX* GetFormat() const
		{
			return m_pwfx;
		};
		/*!
		* @brief	ファイルパスのハッシュ値を取得。
		*/
		unsigned int GetFilePathHash() const
		{
			return m_filePathHash;
		}
	private:
		HMMIO			m_hmmio = NULL;	//Waveファイルハンドル。
		WAVEFORMATEX* 	m_pwfx = NULL;  //waveファイルのフォーマット定義。
		MMCKINFO 		m_ckRiff;      // Use in opening a WAVE file
		DWORD			m_dwSize = 0;      // The size of the wave file
		MMCKINFO		m_ck;          // Multimedia RIFF chunk
		BYTE*			m_pbData = NULL;
		BYTE*			m_pbDataCur = NULL;
		ULONG			m_ulDataSize = 0;
		bool			m_isReadEnd = true;	//読み込み終了フラグ。
		std::thread		m_readAsyncThread;	//非同期読み込みスレッド。
		bool			m_isInvokeReadAsyncThread = false;
		std::string		m_filePath;			//ファイルパス。
		unsigned int	m_filePathHash = 0;		//ファイルパスのハッシュコード。
	};
}