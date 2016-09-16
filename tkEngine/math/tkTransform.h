/*!
 *@brief	トランスフォームクラス。
 */

#ifndef _TKTRANSFORM_H_
#define _TKTRANSFORM_H_

namespace tkEngine{
	/*!
	 *@brief	トランスフォームクラス。
	 */
	class CTransform{
	public:
		CVector			m_translate;	//!<平行移動。
		CQuaternion		m_rotation;		//!<回転。
		
	};
}

#endif // _TKTRANSFORM_H_
