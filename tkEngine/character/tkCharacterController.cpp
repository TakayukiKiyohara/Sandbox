/*!
* @brief	キャラクタのコリジョンコントロール。
*/

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/character/tkCharacterController.h"
#include "tkEngine/character/tkCollisionAttr.h"
namespace tkEngine {

	namespace {
		//衝突したときに呼ばれる関数オブジェクト(地面用)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;									//衝突フラグ。
			CVector3 hitPos = CVector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
			CVector3 startPos = CVector3::Zero;					//レイの始点。
			CVector3 hitNormal = CVector3::Zero;				//衝突点の法線。
			btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
			float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。

																//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
					) {
					//自分に衝突した。or キャラクタ属性のコリジョンと衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
				//上方向と法線のなす角度を求める。
				float angle = hitNormalTmp.Dot(CVector3::Up);
				angle = fabsf(acosf(angle));
				if (angle < CMath::PI * 0.3f		//地面の傾斜が54度より小さいので地面とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //もしくはコリジョン属性が地面と指定されている。
					) {
					//衝突している。
					isHit = true;
					CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
					//衝突点の距離を求める。。
					CVector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					if (dist > distTmp) {
						//この衝突点の方が近いので、最近傍の衝突点を更新する。
						hitPos = hitPosTmp;
						hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
						dist = distTmp;
					}
				}
				return 0.0f;
			}
		};
		//衝突したときに呼ばれる関数オブジェクト(壁用)
		struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//衝突フラグ。
			CVector3 hitPos = CVector3::Zero;		//衝突点。
			CVector3 startPos = CVector3::Zero;		//レイの始点。
			float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
			CVector3 hitNormal = CVector3::Zero;	//衝突点の法線。
			btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
													//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me) {
					//自分に衝突した。or 地面に衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				CVector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//上方向と衝突点の法線のなす角度を求める。
				float angle = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up)));
				if (angle >= CMath::PI * 0.3f		//地面の傾斜が54度以上なので壁とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//もしくはコリジョン属性がキャラクタなので壁とみなす。
					) {
					isHit = true;
					CVector3 hitPosTmp;
					hitPosTmp.Set(convexResult.m_hitPointLocal);
					//交点との距離を調べる。
					CVector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					vDist.y = 0.0f;
					float distTmp = vDist.Length();
					if (distTmp < dist) {
						//この衝突点の方が近いので、最近傍の衝突点を更新する。
						hitPos = hitPosTmp;
						dist = distTmp;
						hitNormal = hitNormalTmp;
					}
				}
				return 0.0f;
			}
		};
	}


	void CCharacterController::Init(float radius, float height, const CVector3& position)
	{
		m_position = position;
		//コリジョン作成。
		m_radius = radius;
		m_height = height;
		m_collider.Create(radius, height);

		//剛体を初期化。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Create(rbInfo);
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		//@todo 未対応。trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		PhysicsWorld().AddRigidBody(&m_rigidBody);

	}
	void CCharacterController::Execute()
	{
		//速度に重力加速度を加える。
		m_moveSpeed.y += m_gravity * GameTime().GetFrameDeltaTime();
		//次の移動先となる座標を計算する。
		CVector3 nextPosition = m_position;
		//速度からこのフレームでの移動量を求める。オイラー積分。
		CVector3 addPos = m_moveSpeed;
		addPos.Scale(GameTime().GetFrameDeltaTime());
		nextPosition.Add(addPos);
		CVector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//XZ平面での衝突検出と衝突解決を行う。
		{
			int loopCount = 0;
			while (true) {
				//現在の座標から次の移動先へ向かうベクトルを求める。
				CVector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				CVector3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (addPosXZ.Length() < FLT_EPSILON) {
					//XZ平面で動きがないので調べる必要なし。
					//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
					//とても小さい値のことです。
					break;
				}
				//カプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				CVector3 posTmp = m_position;
				posTmp.y += m_height * 0.5f + m_radius + 0.2f;
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				SweepResultWall callback;
				callback.me = m_rigidBody.GetBody();
				callback.startPos = posTmp;
				//衝突検出。
				PhysicsWorld().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

				if (callback.isHit) {
					//当たった。
					//壁。
					CVector3 vT0, vT1;
					//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
					//めり込みが発生している移動ベクトルを求める。
					CVector3 vMerikomi;
					vMerikomi.Subtract(vT0, vT1);
					//XZ平面での衝突した壁の法線を求める。。
					CVector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//めり込みベクトルを壁の法線に射影する。
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//押し戻し返すベクトルを求める。
					//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
					CVector3 vOffset;
					vOffset = hitNormalXZ;
					vOffset.Scale(-fT0 + m_radius);
					nextPosition.Add(vOffset);
					CVector3 currentDir;
					currentDir.Subtract(nextPosition, m_position);
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//角に入った時のキャラクタの振動を防止するために、
						//移動先が逆向きになったら移動をキャンセルする。
						nextPosition.x = m_position.x;
						nextPosition.z = m_position.z;
						break;
					}
				}
				else {
					//どことも当たらないので終わり。
					break;
				}
				loopCount++;
				if (loopCount == 5) {
					break;
				}
			}
		}
		//XZの移動は確定。
		m_position.x = nextPosition.x;
		m_position.z = nextPosition.z;
		//下方向を調べる。
		{
			CVector3 addPos;
			addPos.Subtract(nextPosition, m_position);

			m_position = nextPosition;	//移動の仮確定。
										//レイを作成する。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心。
			start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
			//終点は地面上にいない場合は1m下を見る。
			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
			CVector3 endPos;
			endPos.Set(start.getOrigin());
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//ジャンプ中とかで上昇中。
					//上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					//落下している場合はそのまま下を調べる。
					endPos.y += addPos.y;
				}
			}
			else {
				//地面上にいない場合は1m下を見る。
				endPos.y -= 1.0f;
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			SweepResultGround callback;
			callback.me = m_rigidBody.GetBody();
			callback.startPos.Set(start.getOrigin());
			//衝突検出。
			PhysicsWorld().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.isHit) {
				//当たった。
			/*	CVector3 Circle;
				float x = 0.0f;
				float offset = 0.0f;	//押し戻す量。
				Circle = CVector3::Zero;

				Circle = m_position;
				Circle.y = callback.hitPos.y;//円の中心
				CVector3 v;
				v.Subtract(Circle, callback.hitPos);
				x = v.Length();//物体の角とプレイヤーの間の横幅の距離が求まる。

				offset = sqrt(max(0.0f, m_radius*m_radius - x*x));//yの平方根を求める。
				*/
				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;//; + offset - m_radius;
			}
			else {
				//地面上にいない。
				m_isOnGround = false;
			}
		}
		//移動確定。
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
		//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	}
	/*!
	* @brief	死亡したことを通知。
	*/
	void CCharacterController::RemoveRigidBoby()
	{
		PhysicsWorld().RemoveRigidBody(&m_rigidBody);
	}
}
