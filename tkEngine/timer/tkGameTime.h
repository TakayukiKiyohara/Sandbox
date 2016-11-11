/*!
 *@brief	ゲーム時間クラス。
 * シングルトン。
 */
#pragma once

#include "tkEngine/timer/tkStopwatch.h"

namespace tkEngine{
	/*!
	 *@brief	ゲーム時間。
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
		 *@brief	1フレームの経過時間を取得(単位・秒)。
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
		float		m_frameDeltaTime = 1.0f / 60.0f;		//1フレームの経過時間。
	};
	static CGameTime& GameTime()
	{
		return CGameTime::GetInstance();
	}
}
