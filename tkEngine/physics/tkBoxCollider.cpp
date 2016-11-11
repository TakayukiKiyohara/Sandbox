#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkBoxCollider.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CBoxCollider::CBoxCollider() :
		shape(NULL)
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CBoxCollider::~CBoxCollider()
	{
		delete shape;
	}
	/*!
	 * @brief	ボックスコライダーを作成。
	 */
	void CBoxCollider::Create( const CVector3& size )
	{
		shape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	}
}