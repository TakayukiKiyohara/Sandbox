/*!
 * @brief	球体コライダー。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkSphereCollider.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CSphereCollider::CSphereCollider() :
		shape(NULL)
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CSphereCollider::~CSphereCollider()
	{
		delete shape;
	}
	/*!
	 * @brief	球体コライダーを作成。
	 */
	void CSphereCollider::Create( const float radius )
	{
		shape = new btSphereShape(radius);
	}
}