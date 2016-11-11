#pragma once

#include "tkEngine/Physics/tkICollider.h"

namespace tkEngine{
	/*!
	 * @brief	�{�b�N�X�R���C�_�[�B
	 */
	class CBoxCollider : public ICollider
	{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CBoxCollider();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CBoxCollider();
		/*!
		 * @brief	�{�b�N�X�R���C�_�[���쐬�B
		 */
		void Create( const CVector3& size );
		btCollisionShape* GetBody() override
		{
			return shape;
		}
	private:
		btBoxShape*		shape;
	};
}