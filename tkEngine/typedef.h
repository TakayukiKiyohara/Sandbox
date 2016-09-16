#ifndef TK_ENGINE_TYPEDEF_H
#define TK_ENGINE_TYPEDEF_H

/*!
 * @brief	基本型別名定義
 */

#include <climits>
#include <float.h>
typedef uintptr_t	UIntPtr;				
// 変数の限界
static const int S32_MAX = INT_MAX; 				//!< 符号あり32bit整数の最大値
static const int S32_MIN = INT_MIN; 				//!< 符号あり32bit整数の最小値
static const int U32_MAX = UINT_MAX;				//!< 符号無し32bit整数の最大値

#define		r_cast		reinterpret_cast
#define		s_cast		static_cast
#define		c_cast		const_cast

#endif // #ifndef TK_ENGINE_TYPEDEF_H
