/*!
 *@brief	�Q�[�����ԃN���X�B
 * �V���O���g���B
 */
#pragma once

#include "tkEngine/timer/tkStopwatch.h"

namespace tkEngine{
	/*!
	 *@brief	�Q�[�����ԁB
	 */
	class CGameTime{
		CGameTime() 
		{
		}
		~CGameTime()
		{
		}
	public:
		static CGameTime& GetInstance()
		{
			static CGameTime t;
			return t;
		}
		/*!
		 *@brief	1�t���[���̌o�ߎ��Ԃ��擾(�P�ʁE�b)�B
		 */
		float GetFrameDeltaTime() const
		{
			return m_frameDeltaTime;
		}
		void SetFrameDeltaTime(float deltaTime) 
		{
			m_frameDeltaTime = min( 1.0f/20.0f, deltaTime );
		}
		
	private:
		float		m_frameDeltaTime = 1.0f / 60.0f;		//1�t���[���̌o�ߎ��ԁB
	};
	static CGameTime& GameTime()
	{
		return CGameTime::GetInstance();
	}
}
