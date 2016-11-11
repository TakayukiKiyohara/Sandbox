#pragma once

#include "tkEngine/Physics/tkMeshCollider.h"
#include "tkEngine/Physics/tkRigidBody.h"

class Ground : public IGameObject {
	CSkinModelData	skinModelData;		//スキンモデルデータ。
	CSkinModel		skinModel;			//スキンモデル。
	CAnimation		animation;			//アニメーション。
	CLight			light;				//ライト。
	CTexture		normalMap;
	CMatrix*		m_worldMatrix;		//ワールド行列
	CMeshCollider	meshCollider;		//メッシュコライダー。
	CRigidBody		rigidBody;			//剛体。
public:
	Ground()
	{
		
	}
	~Ground()
	{

	}
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
	//地面との当たり判定。
	//pos	レイの始点。
	//ray	レイ方向
	//bHit	レイと地面が衝突したら1が設定される。
	//len	レイの始点から交点までの距離。
	void IsIntersect(const CVector3& pos, const CVector3& ray, int& bHit, float& len);
private:
};
extern Ground* g_ground;