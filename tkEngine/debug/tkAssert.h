/*!
 *@brief	�A�T�[�g
 */

#ifndef _TKASSERT_H_
#define _TKASSERT_H_

#include <stdarg.h>

namespace tkEngine{
	/*!
	 *@brief	�A�T�[�g
	 *@param[in]	flag	�U�̂Ƃ��A�T�[�g���������܂��B
	 */
	static inline void Assert( bool flag, const char* format, const char* file, long line, ... )
	{
		if(!flag){
			va_list va;
			va_start( va, flag );
			vprintf( format, va );
			char fileLineInfo[256];
			sprintf(fileLineInfo, "%s, %d�s��", file, line );
			char assertMessage[256];
			vsprintf(assertMessage, format, va);
			strcat(assertMessage, fileLineInfo);
			MessageBox(nullptr, assertMessage, "�A�T�[�g", MB_OK);
			va_end( va );
			std::abort();
		}
	}
}
#ifdef _DEBUG
#define 	TK_ASSERT( flg, format, ... )	tkEngine::Assert( flg, format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define 	TK_ASSERT( flg, format, ... )
#endif 
#endif