/*!
 * @brief	数学関係色々。
 */

#ifndef _TK_MATH_H_
#define _TK_MATH_H_

namespace tkEngine{
	class CMath{
	public:
		static const float PI;
		/*!
		 * @brief	degreeからradianに変換。
		 */
		static float DegToRad( float deg )
		{
			return deg * (PI/180.0f);
		}
		/*!
		 * @brief	radianからdegreeに変換。
		 */
		static float RadToDeg( float rad )
		{
			return rad / (PI/180.0f);
		}
	};
}

#endif //_TK_MATH_H_