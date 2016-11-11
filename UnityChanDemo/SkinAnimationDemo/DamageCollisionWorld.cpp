/*!
 * @brief	�G�A�v���C���[�Ƀ_���[�W��^����R���W�������[���h�B
 */

#include "stdafx.h"
#include "DamageCollisionWorld.h"

namespace {
	struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback
	{
		MyContactResultCallback() 
		{

		}
		btCollisionObject* queryCollisionObject = nullptr;
		DamageCollisionWorld:: Collision* hitObject = nullptr;
		float distSq = FLT_MAX;
		DamageCollisionWorld::EnAttr queryAttr = DamageCollisionWorld::enDamageToEnemy;	//���ׂ�R���W���������B
		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{
			const CVector3* vColl0Pos = (CVector3*)(&colObj0Wrap->getWorldTransform().getOrigin());
			const CVector3* vColl1Pos = (CVector3*)(&colObj1Wrap->getWorldTransform().getOrigin());
			CVector3 vDist;
			vDist.Subtract(*vColl0Pos, *vColl1Pos);
			float distTmpSq = vDist.LengthSq();
			DamageCollisionWorld::Collision* hitObjectTmp;
			if (distTmpSq < distSq) {
				//������̕����߂��B
				if (queryCollisionObject == colObj0Wrap->getCollisionObject()) {
					hitObjectTmp = (DamageCollisionWorld::Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
				}
				else {
					hitObjectTmp = (DamageCollisionWorld::Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
				}
				if (hitObjectTmp->attr == queryAttr) {
					distSq = distTmpSq;
					hitObject = hitObjectTmp;
				}
			}
			
			return 0.0f;
		}
	};
}
DamageCollisionWorld::DamageCollisionWorld()
{
}

DamageCollisionWorld::~DamageCollisionWorld()
{
}
void DamageCollisionWorld::Start()
{
	collisionConfig.reset( new btDefaultCollisionConfiguration );
	collisionDispatcher.reset( new btCollisionDispatcher(collisionConfig.get()) );
	overlappingPairCache.reset(new btDbvtBroadphase() );
	collisionWorld.reset(new btCollisionWorld(collisionDispatcher.get(), overlappingPairCache.get(), collisionConfig.get()));
}
/*!
*@brief	�_���[�W�R���W�����Əd�Ȃ��Ă��邩���ׂ�B
*/
const DamageCollisionWorld::Collision* DamageCollisionWorld::FindOverlappedDamageCollision(EnAttr attr, const CVector3& pos, float radius) const
{
	for (auto& collision : collisions) {
		if (collision->attr == attr) {
			float t = collision->radius + radius;
			CVector3 diff;
			diff.Subtract(collision->position, pos);
			if (diff.LengthSq() < t * t) {
				//���������B
				return collision.get();
			}
		}
	}
	return nullptr;
}
const DamageCollisionWorld::Collision* DamageCollisionWorld::FindOverlappedDamageCollision(EnAttr attr, btCollisionObject* colliObject) const
{
	MyContactResultCallback callback;
	callback.queryCollisionObject = colliObject;
	callback.queryAttr = attr;
	collisionWorld->contactTest(colliObject, callback);

	return callback.hitObject;
}
void DamageCollisionWorld::Update()
{
	std::list<CollisionPtr>::iterator it = collisions.begin();
	while(it != collisions.end()){
		(*it)->time += GameTime().GetFrameDeltaTime();
		if((*it)->time > (*it)->life){
			//���S�B
			collisionWorld->removeCollisionObject((*it)->collisionObject.get());
			it = collisions.erase(it);
		}else{
			it++;
		}
	}
	collisionWorld->updateAabbs();
}
/*!
*@brief	�`��
*/
void DamageCollisionWorld::Render(CRenderContext& renderContext) 
{
#ifdef DEBUG_DMG_COLLISION_DRAW
	for (CollisionPtr coll : collisions) {
		CMatrix mWVP;
		mWVP.MakeTranslation(coll->position);
		mWVP.Mul(mWVP, g_camera->GetCamera().GetViewProjectionMatrix());
		coll->debugShape.RenderLightWVP(renderContext, mWVP);
	}
#endif
}