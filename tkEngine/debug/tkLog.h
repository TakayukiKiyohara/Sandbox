/*!
 *@brief		ÉçÉO
 */

#ifndef _TKLOG_H_
#define _TKLOG_H_


namespace tkEngine{
	/*!
	 *@brief	ÉçÉOèoóÕ
	 */
	static inline void Log( const char* format, ... )
	{
		char log[256];
		va_list va;
		va_start( va, format );
		vsprintf( log, format, va );
		OutputDebugString( log );
		va_end( va );
	}
}

#ifdef _DEBUG
	#define TK_LOG( format, ... )	Log(format, __VA_ARGS__)
#else // _DEBUG
	#define TK_LOG( format, ... )	
#endif // _DEBUG
#endif // _TKLOG_H_