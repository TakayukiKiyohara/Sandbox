#ifndef TK_ENGINE_TYPEDEF_H
#define TK_ENGINE_TYPEDEF_H

/*!
 * @brief	��{�^�ʖ���`
 */

#include <climits>
#include <float.h>
typedef uintptr_t	UIntPtr;				
// �ϐ��̌��E
static const int S32_MAX = INT_MAX; 				//!< ��������32bit�����̍ő�l
static const int S32_MIN = INT_MIN; 				//!< ��������32bit�����̍ŏ��l
static const int U32_MAX = UINT_MAX;				//!< ��������32bit�����̍ő�l

#define		r_cast		reinterpret_cast
#define		s_cast		static_cast
#define		c_cast		const_cast

#endif // #ifndef TK_ENGINE_TYPEDEF_H
