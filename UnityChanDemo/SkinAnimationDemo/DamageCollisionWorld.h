/*!
 * @brief	�G�A�v���C���[�Ƀ_���[�W��^����R���W�������[���h�B
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
	* @brief	�����B
	*/
	enum EnAttr {
		enDamageToEnemy,		//�G�Ƀ_���[�W��^����R���W�����B
		enDamageToPlayer,		//�v���C���[�Ƀ_���[�W��^����R���W�����B
	};
	class Collision {
	public:
		float radius;						//���a�B
		CVector3 position;					//���S�B
		float life;							//�����B
		int damage;							//�_���[�W�ʁB
		EnAttr attr;						//�����B
		int groupID;						//�R���W�����̃O���[�v�h�c�B
		float time;
		std::shared_ptr<btCollisionObject>	collisionObject;	//�R���W�����I�u�W�F�N�g�B
		CSphereCollider						sphereCollider;		//�X�t�B�A�R���C�_�[�B
#ifdef DEBUG_DMG_COLLISION_DRAW
		CSphereShape debugShape;
#endif
	};
	
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	DamageCollisionWorld();
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~DamageCollisionWorld();
	/*!
	 * @brief	�R���W������ǉ��B
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
	 * @brief	�X�V�B
	 */
	void Update() override;
	/*!
	*@brief	�`��
	*/
	void Render(CRenderContext& renderContext) override;
	/*!
	*@brief	�d�Ȃ��Ă���_���[�W�R���W�������擾����B
	*/
	const Collision* FindOverlappedDamageCollision(EnAttr attr, const CVector3& pos, float radius) const;
	const Collision* FindOverlappedDamageCollision(EnAttr attr, btCollisionObject* colliObject) const;
private:
	typedef std::shared_ptr<Collision> CollisionPtr;
	std::list<CollisionPtr>								collisions;
	std::unique_ptr<btCollisionWorld>					collisionWorld;
	std::unique_ptr<btDefaultCollisionConfiguration>	collisionConfig;
	std::unique_ptr<btCollisionDispatcher>				collisionDispatcher;	//!<�Փˉ��������B
	std::unique_ptr<btBroadphaseInterface>				overlappingPairCache;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
};

extern DamageCollisionWorld* g_damageCollisionWorld;