/*!
 * @brief	�R���C�_�[�B
 */

#pragma once
namespace tkEngine{
	/*!
	 * @brief	�R���C�_�[�̃C���^�[�t�F�[�X�N���X�B
	 */
	class ICollider {
	public:
		virtual btCollisionShape* GetBody() = 0;
	};
}