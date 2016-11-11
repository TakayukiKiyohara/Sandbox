/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkCapsuleCollider.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CCapsuleCollider::CCapsuleCollider() :
		shape(nullptr)
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CCapsuleCollider::~CCapsuleCollider()
	{
		delete shape;
	}
}