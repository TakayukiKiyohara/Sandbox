/*!
 * @brief	敵、プレイヤーにダメージを与えるコリジョンワールド。
 */

#pragma once

#include "tkEngine/Physics/tkSphereCollider.h"
#include "tkEngine/shape/tkSphereShape.h"

#ifdef _DEBUG
#define DEBUG_DMG_COLLISION_DRAW
#else
//%#define DEBUG_DMG_COLLISION_DRAW
#endif
class DamageCollisionWorld : public IGameObject{
public:
	/*!
	* @brief	属性。
	*/
	enum EnAttr {
		enDamageToEnemy,		//敵にダメージを与えるコリジョン。
		enDamageToPlayer,		//プレイヤーにダメージを与えるコリジョン。
	};
	class Collision {
	public:
		float radius;						//半径。
		CVector3 position;					//中心。
		float life;							//寿命。
		int damage;							//ダメージ量。
		EnAttr attr;						//属性。
		int groupID;						//コリジョンのグループＩＤ。
		float time;
		std::shared_ptr<btCollisionObject>	collisionObject;	//コリジョンオブジェクト。
		CSphereCollider						sphereCollider;		//スフィアコライダー。
#ifdef DEBUG_DMG_COLLISION_DRAW
		CSphereShape debugShape;
#endif
	};
	
	/*!
	 * @brief	コンストラクタ。
	 */
	DamageCollisionWorld();
	/*!
	 * @brief	デストラクタ。
	 */
	~DamageCollisionWorld();
	/*!
	 * @brief	コリジョンを追加。
	 */
	void Add(
		float radius,
		const CVector3& pos,
		float life,
		int damage,
		EnAttr attr,
		int groupID
	)
	{
		CollisionPtr colli = CollisionPtr(new Collision);
		colli->radius = radius;
		colli->position = pos;
		colli->life = life;
		colli->attr = attr;
		colli->time = 0.0f;
		colli->damage = damage;
		colli->groupID = groupID;
		colli->sphereCollider.Create(radius);
		colli->collisionObject.reset(new btCollisionObject);
		colli->collisionObject->setCollisionShape(colli->sphereCollider.GetBody());
		btTransform worldTrans;
		worldTrans.setIdentity();
		worldTrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
		colli->collisionObject->setWorldTransform(worldTrans);
		colli->collisionObject->setUserPointer(colli.get());
		collisions.push_back(colli);
#ifdef DEBUG_DMG_COLLISION_DRAW
		colli->debugShape.Create(radius, 10, 0xFFFF0000, true);
#endif
		collisionWorld->addCollisionObject(colli->collisionObject.get());
	}
	void Start() override;
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	/*!
	*@brief	描画
	*/
	void Render(CRenderContext& renderContext) override;
	/*!
	*@brief	重なっているダメージコリジョンを取得する。
	*/
	const Collision* FindOverlappedDamageCollision(EnAttr attr, const CVector3& pos, float radius) const;
	const Collision* FindOverlappedDamageCollision(EnAttr attr, btCollisionObject* colliObject) const;
private:
	typedef std::shared_ptr<Collision> CollisionPtr;
	std::list<CollisionPtr>								collisions;
	std::unique_ptr<btCollisionWorld>					collisionWorld;
	std::unique_ptr<btDefaultCollisionConfiguration>	collisionConfig;
	std::unique_ptr<btCollisionDispatcher>				collisionDispatcher;	//!<衝突解決処理。
	std::unique_ptr<btBroadphaseInterface>				overlappingPairCache;	//!<ブロードフェーズ。衝突判定の枝切り。
};

extern DamageCollisionWorld* g_damageCollisionWorld;