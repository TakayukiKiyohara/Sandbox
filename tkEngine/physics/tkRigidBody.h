#pragma once

namespace tkEngine{

	class ICollider;

	//剛体情報。
	struct RigidBodyInfo {
		RigidBodyInfo() :
			collider(NULL),
			mass(0.0f)
		{
			pos = CVector3::Zero;
			rot = CQuaternion::Identity;
		}
		CVector3 pos;				//座標。
		CQuaternion rot;			//回転。
		ICollider* collider;		//コライダー。
		float mass;					//質量。

	};
	//剛体クラス。
	class CRigidBody
	{
		btRigidBody*			rigidBody;		//剛体。
		btDefaultMotionState*	myMotionState;	//モーションステート。
	public:
		CRigidBody();
		~CRigidBody();
		void Release();
		void Create(RigidBodyInfo& rbInfo);
		btRigidBody* GetBody()
		{
			return rigidBody;
		}
	};
}
