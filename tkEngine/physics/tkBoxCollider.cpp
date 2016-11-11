#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkBoxCollider.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CBoxCollider::CBoxCollider() :
		shape(NULL)
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CBoxCollider::~CBoxCollider()
	{
		delete shape;
	}
	/*!
	 * @brief	�{�b�N�X�R���C�_�[���쐬�B
	 */
	void CBoxCollider::Create( const CVector3& size )
	{
		shape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	}
}