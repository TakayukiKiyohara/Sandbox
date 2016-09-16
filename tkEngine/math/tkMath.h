/*!
 * @brief	���w�֌W�F�X�B
 */

#ifndef _TK_MATH_H_
#define _TK_MATH_H_

namespace tkEngine{
	class CMath{
	public:
		static const float PI;
		/*!
		 * @brief	degree����radian�ɕϊ��B
		 */
		static float DegToRad( float deg )
		{
			return deg * (PI/180.0f);
		}
		/*!
		 * @brief	radian����degree�ɕϊ��B
		 */
		static float RadToDeg( float rad )
		{
			return rad / (PI/180.0f);
		}
	};
}

#endif //_TK_MATH_H_