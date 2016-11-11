#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Physics/tkRigidBody.h"
#include "tkEngine/Physics/tkICollider.h"

namespace tkEngine{
	CRigidBody::CRigidBody() :
		rigidBody(NULL),
		myMotionState(NULL)
	{
	}


	CRigidBody::~CRigidBody()
	{
		Release();
	}
	void CRigidBody::Release()
	{
		delete rigidBody;
		delete myMotionState;
		rigidBody = NULL;
		myMotionState = NULL;
	}
	void CRigidBody::Create(RigidBodyInfo& rbInfo)
	{
		Release();
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(rbInfo.pos.x, rbInfo.pos.y, rbInfo.pos.z));
		transform.setRotation(btQuaternion(rbInfo.rot.x, rbInfo.rot.y, rbInfo.rot.z, rbInfo.rot.w));
		myMotionState = new btDefaultMotionState;
		btRigidBody::btRigidBodyConstructionInfo btRbInfo(rbInfo.mass, myMotionState, rbInfo.collider->GetBody(), btVector3(0, 0, 0));
		//„‘Ì‚ğì¬B
		rigidBody = new btRigidBody(btRbInfo);
	}
}