/*!
 * @brief	コライダー。
 */

#pragma once
namespace tkEngine{
	/*!
	 * @brief	コライダーのインターフェースクラス。
	 */
	class ICollider {
	public:
		virtual btCollisionShape* GetBody() = 0;
	};
}