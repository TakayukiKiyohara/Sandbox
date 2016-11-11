/*!
 * @brief	�T�E���h�G���W��
 */

#pragma once
#include "tkEngine/Sound/tkWaveFileBank.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//�ő�o�̓`�����l�����B
namespace tkEngine{
	class CWaveFile;
	class CSoundSource;
	/*!
	 * @brief	�T�E���h�G���W���B
	 */
	class CSoundEngine{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CSoundEngine();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CSoundEngine();
		/*!
		 * @brief	�������B
		 */
		void Init();
		/*!
		 * @brief	�J���B
		 */
		void Release();
		/*!
		* @brief	�X�V�B
		*/
		void Update();
		/*!
		* @brief	3D�T�E���h�\�[�X��ǉ��B
		*/
		void Add3DSoundSource(CSoundSource* s)
		{
			m_3dSoundSource.push_back(s);
		}
		/*!
		* @brief	3D�T�E���h�\�[�X���폜�B
		*/
		void Remove3DSoundSource(CSoundSource* s)
		{
			auto& it = std::find(m_3dSoundSource.begin(), m_3dSoundSource.end(), s);
			if (it != m_3dSoundSource.end()) {
				m_3dSoundSource.erase(it);
			}
		}
		/*!
		* @brief	�T�E���h���X�i�[�̍��W��ݒ�B
		* @details
		*  3D�������g���Ă��鎞�̌v�Z�Ɏg�p����܂��B
		*/
		void SetListenerPosition(const CVector3& pos)
		{
			m_listenerPosition = pos;
		}
		/*!
		* @brief	�T�E���h���X�i�[�̑O������ݒ�B
		* @details
		*  3D�������g���Ă��鎞�̌v�Z�Ɏg�p����܂��B
		*/
		void SetListenerFront(CVector3 front)
		{
			m_listener.OrientFront.x = front.x;
			m_listener.OrientFront.y = front.y;
			m_listener.OrientFront.z = front.z;
		}
		/*!
		* @brief	�T�E���h���X�i�[�̏������ݒ�B
		* @details
		*  3D�������g���Ă��鎞�̌v�Z�Ɏg�p����܂��B
		*/
		void SetListenerUp(CVector3 up)
		{
			m_listener.OrientTop.x = up.x;
			m_listener.OrientTop.y = up.y;
			m_listener.OrientTop.z = up.z;
		}
		/*!
		* @brief	XAudio2�̃\�[�X�{�C�X���쐬�B
		*/
		IXAudio2SourceVoice* CreateXAudio2SourceVoice(CWaveFile* waveFile, bool is3DSound);
		IXAudio2MasteringVoice* GetMasteringVoice()
		{
			return m_masteringVoice;
		}
		IXAudio2SubmixVoice* GetSubmixVoice()
		{
			return m_submixVoice;
		}
		/*!
		* @brief	�o�̓`�����l���̐����擾�B
		*/
		int GetNumChannel() const
		{
			return m_nChannels;
		}
		/*!
		* @brief	�g�`�f�[�^�o���N���擾�B
		*/
		CWaveFileBank& GetWaveFileBank()
		{
			return m_waveFileBank;
		}
	private:
		IXAudio2* m_xAudio2 = nullptr;
		IXAudio2MasteringVoice* m_masteringVoice = nullptr;
		X3DAUDIO_HANDLE m_hx3DAudio;					//!<3D�I�[�f�B�I�̃n���h���B
		IUnknown* m_reverbEffect = nullptr;				//!<���o�[�v�G�t�F�N�g�B
		IXAudio2SubmixVoice* m_submixVoice = nullptr;	//!<�T�u�~�b�N�X�{�C�X�B
		DWORD m_channelMask = 0;						//!<�I�[�f�B�I�`�����l���}�X�N�B
		DWORD m_nChannels = 0;							//!<�`�����l�����B
		CVector3 m_listenerPosition = CVector3::Zero;	//!<���X�i�[�̍��W�B
		float m_fListenerAngle = 0.0f;
		bool m_UseListenerCone = false;
		bool m_fUseInnerRadius = false;
		bool m_useRedirectToLFE = false;
		bool m_isInited = false;
		X3DAUDIO_LISTENER m_listener;					//!<�T�E���h���X�i�[�B
		std::list<CSoundSource*>	m_3dSoundSource;	//!<3D�T�E���h�̃T�E���h�\�[�X�B
		X3DAUDIO_CONE	m_emitterCone;
		CWaveFileBank m_waveFileBank;					//!<�g�`�f�[�^�̃o���N�B
	};
}