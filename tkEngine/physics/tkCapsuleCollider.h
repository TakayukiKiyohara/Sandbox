/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#pragma once

#include "tkEngine/Physics/tkICollider.h"

namespace tkEngine{
	class CCapsuleCollider : public ICollider
	{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CCapsuleCollider();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CCapsuleCollider();
		/*!
		* @brief	�쐬�B
		*/
		void Create(float radius, float height)
		{
			shape = new btCapsuleShape(radius, height);
		}
		btCollisionShape* GetBody() override
		{
			return shape;
		}
	private:
		btCapsuleShape*		shape;
	};
}