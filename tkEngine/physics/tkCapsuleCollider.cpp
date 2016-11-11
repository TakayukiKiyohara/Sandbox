/*!
 * @brief	カプセルコライダー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkCapsuleCollider.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CCapsuleCollider::CCapsuleCollider() :
		shape(nullptr)
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CCapsuleCollider::~CCapsuleCollider()
	{
		delete shape;
	}
}