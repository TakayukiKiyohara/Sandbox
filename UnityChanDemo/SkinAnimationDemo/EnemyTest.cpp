#include "stdafx.h"
#include "EnemyTest.h"
#include "Car.h"
#include "Map/Ground.h"

CSkinModelData*	EnemyTest::orgSkinModelData = NULL;	//オリジナルスキンモデルデータ。


namespace {
	CAnimation				*orgAnimation;			//アニメーション。
	const float MAX_RUN_SPEED = 0.1f;					//ユニティちゃんの走りの最高速度。
	const float RUN_THREADHOLD_SQ = 0.07f * 0.07f;		//走りアニメーションを再生する速度の閾値。
	//地面との当たり判定。
	struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit;
		CVector3 hitPos;
		CVector3 startPos;
		float dist;
		CVector3 hitNormal;
		SweepResultGround()
		{
			isHit = false;
			dist = FLT_MAX;
			hitPos = CVector3::Zero;
			startPos = CVector3::Zero;
			hitNormal = CVector3::Zero;
		}

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
			float t = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up)));
			if (t < CMath::PI * 0.3f) {
				isHit = true;
				CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
				//交点との距離を調べる。
				CVector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					hitPos = hitPosTmp;
					dist = distTmp;
					hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
				}
			}
			return 0.0f;
		}
	};
	//壁
	struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit;
		CVector3 hitPos;
		CVector3 startPos;
		float dist;
		CVector3 hitNormal;
		SweepResultWall()
		{
			isHit = false;
			dist = FLT_MAX;
			hitPos = CVector3::Zero;
			startPos = CVector3::Zero;
			hitNormal = CVector3::Zero;
		}

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
			float t = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up)));
			if (t >= CMath::PI * 0.3f) {
				isHit = true;
				CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
				//交点との距離を調べる。
				CVector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					hitPos = hitPosTmp;
					dist = distTmp;
					hitNormal = *(CVector3*)&convexResult.m_hitNormalLocal;
				}
			}
			return 0.0f;
		}
	};
}
/*!
* @brief	開始
*/
void EnemyTest::Start()
{
	if (orgSkinModelData == NULL) {
		orgSkinModelData = new CSkinModelData;
		orgAnimation = new CAnimation;
		//orgSkinModelData->LoadModelData("Assets/modelData/Unity.X", orgAnimation);
		orgSkinModelData->LoadModelData("Assets/modelData/player.X", orgAnimation);
	}
	//オリジナルのモデルデータからクローンモデルを作成。
	skinModelData.CloneModelData(*orgSkinModelData, &animation);
	
	skinModel.Init(&skinModelData);
	skinModel.SetLight(&light);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	skinModel.SetFresnelFlag(true);
	skinModel.SetReflectionCasterFlag(true);

	light.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	light.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, -0.707f));
	light.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, -0.707f));


	light.SetDiffuseLightColor(0, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, CVector4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbinetLight(CVector3(0.4f, 0.4f, 0.4f));

	light.SetLimLightColor(CVector4(0.6f, 0.6f, 0.6f, 1.0f));
	light.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	isPointLightOn = false;
	UpdatePointLightPosition();

	currentAnimSetNo = AnimationInvalid;
	PlayAnimation(currentAnimSetNo);
	rotation = CQuaternion::Identity;

	CVector3 lightPos = CVector3(0.0f, 25.5f, 24.5f);
	ShadowMap().SetLightPosition(lightPos);
	ShadowMap().SetLightTarget(position);
	toLightPos.Subtract(lightPos, position);
	ShadowMap().SetCalcLightViewFunc(CShadowMap::enCalcLightViewFunc_PositionTarget);
	state = enStateStand;
	isJump = false;

	radius = 0.4f;

	collider.Create(radius);
	RigidBodyInfo rbInfo;
	rbInfo.pos = position;
	rbInfo.rot = rotation;
	rbInfo.collider = &collider;
	rigidBody.Create(rbInfo);
	toLampLocalPos.Set( 0.0f, 0.5f, 0.2f);
	//g_physicsWorld->AddRigidBody(&rigidBody);
}
void EnemyTest::Update()
{
	CVector3 nextPosition = position;
	const float MOVE_SPEED = 5.0f;
	if (state == enStateRun || state == enStateStand) {
		if (Pad(0).IsTrigger(enButtonRB3)) {
			isPointLightOn = !isPointLightOn;
		}
		if (Pad(0).IsPress(enButtonA)) {
			//Aボタンが押された。
			//車との距離を調べる。
			CVector3 diff = g_car->GetPosition();
			diff.Subtract(position);
			if (diff.Length() < 2.0f) {
				//車との距離が2m以内。
				state = enState_RideOnCar;
				skinModel.SetShadowReceiverFlag(false);
				skinModel.SetShadowCasterFlag(false);
				g_car->SetRideOnFlag(true);
				g_camera->SetCar(g_car);
				return;
			}
			else if(!isJump){
				//車との距離が離れていたらジャンプ。
				moveSpeed.y = 8.0f;
				isJump = true;
			}
		}
		//走りか立ち状態の時。
		CVector3 moveDirLocal;
		moveDirLocal.y = 0.0f;
		moveDirLocal.x = Pad(0).GetLStickXF();
		moveDirLocal.z = Pad(0).GetLStickYF();
		const CMatrix& mViewInv = g_camera->GetCamera().GetViewMatrixInv();
		//カメラ空間から見た奥方向のベクトルを取得。
		CVector3 cameraZ;
		cameraZ.x = mViewInv.m[2][0];
		cameraZ.y = 0.0f;		//Y軸いらない。
		cameraZ.z = mViewInv.m[2][2];
		cameraZ.Normalize();	//Y軸を打ち消しているので正規化する。
								//カメラから見た横方向のベクトルを取得。
		CVector3 cameraX;
		cameraX.x = mViewInv.m[0][0];
		cameraX.y = 0.0f;		//Y軸はいらない。
		cameraX.z = mViewInv.m[0][2];
		cameraX.Normalize();	//Y軸を打ち消しているので正規化する。

		CVector3 moveDir;
		moveDir.x = cameraX.x * moveDirLocal.x + cameraZ.x * moveDirLocal.z;
		moveDir.y = 0.0f;	//Y軸はいらない。
		moveDir.z = cameraX.z * moveDirLocal.x + cameraZ.z * moveDirLocal.z;

		moveSpeed.x = moveDir.x * MOVE_SPEED;
		moveSpeed.z = moveDir.z * MOVE_SPEED;
		
		//Y方向には重力落下を加える。
		const float GRAVITY = -18.8f;
		moveSpeed.y += GRAVITY * GameTime().GetFrameDeltaTime();
		CVector3 addPos = moveSpeed;
		addPos.Scale(GameTime().GetFrameDeltaTime());
		nextPosition.Add(addPos);
		if (moveDir.LengthSq() > 0.0001f) {
			rotation.SetRotation(CVector3::Up, atan2f(moveDir.x, moveDir.z));
			//走り状態に遷移。
			state = enStateRun;
		}
		else {
			//立ち状態。
			state = enStateStand;
		}
		
		ShadowMap().SetLightTarget(position);
		CVector3 lightPos;
		lightPos.Add(position, toLightPos);
		ShadowMap().SetLightPosition(lightPos);
		//コリジョン検出と解決を行う。
		CollisionDetectAndResolve(nextPosition);
	}
	else if (state == enState_RideOnCar) {
		ShadowMap().SetLightTarget(g_car->GetPosition());
		CVector3 lightPos;
		lightPos.Add(g_car->GetPosition(), toLightPos);
		ShadowMap().SetLightPosition(lightPos);
		if (g_car->GetMoveSpeed().Length() < 0.1f) {
			//車が停止状態。
			if (Pad(0).IsPress(enButtonB)) {
				//降車。
				g_camera->SetCar(NULL);
				g_car->SetRideOnFlag(false);
				skinModel.SetShadowReceiverFlag(true);
				skinModel.SetShadowCasterFlag(true);
				position = g_car->GetPosition();
				state = enStateStand;
			}
		}
	}
	
	skinModel.Update(position, rotation, CVector3::One);
	
	//ポイントライトの位置を更新。
	UpdatePointLightPosition();
	//アニメーションコントロール。
	AnimationControl();
	lastFrameState = state;
}
/*!
* @brief	ポイントライトの位置を更新。
*/
void EnemyTest::UpdatePointLightPosition()
{
	if (isPointLightOn) {
		pointLightColor.Set(0.9f, 0.75f, 0.6f, 1.0f);
	}
	else {
		pointLightColor = CVector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	light.SetPointLightColor(pointLightColor);
	pointLightPosition = toLampLocalPos;
	CMatrix mWorld = skinModel.GetWorldMatrix();
	mWorld.Mul(pointLightPosition);
	light.SetPointLightPosition(pointLightPosition);
}
/*!
* @brief	衝突検出と解決。
*@param[in]	nextPosition		次の座標。
*/
void EnemyTest::CollisionDetectAndResolve(CVector3 nextPosition)
{
	//XZ平面を調べる。
	{
		int loopCount = 0;
		while (true) {
			CVector3 addPos;
			addPos.Subtract(nextPosition, position);
			CVector3 posTmp = position;
			posTmp.y += radius + 0.2f;
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(*(btVector3*)(&posTmp));
			CVector3 newPos;
			SweepResultWall callback;
			callback.startPos = position;
			CVector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() > 0.0001f) {
				newPos.Add(posTmp, addPosXZ);
				end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));

				PhysicsWorld().ConvexSweepTest((const btConvexShape*)collider.GetBody(), start, end, callback);
			}
			if (callback.isHit) {
				//当たった。
				float t = fabsf(acosf(callback.hitNormal.Dot(CVector3::Up)));
				if (t >= CMath::PI * 0.3f) {
					//壁。
					nextPosition.x = callback.hitPos.x;
					nextPosition.z = callback.hitPos.z;
					//半径分押し戻す。
					CVector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					CVector3 t = hitNormalXZ;
					t.Scale(radius);
					nextPosition.Add(t);
					//続いて壁に沿って滑らせる。
					t.Cross(hitNormalXZ, CVector3::Up);
					t.Normalize();
					//押し戻しで動いた分は減算する。
					CVector3 t2;
					t2.Subtract(nextPosition, position);
					t2.y = 0.0f;
					addPosXZ.Subtract(t2);
					t.Scale(t.Dot(addPosXZ));
					nextPosition.Add(t);
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
	//下方向を調べる。
	{
		CVector3 addPos;
		addPos.Subtract(nextPosition, position);
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(position.x, position.y + radius, position.z));
		CVector3 newPos;
		SweepResultGround callback;
		callback.startPos = position;
		if (addPos.y < 0.0f) {
			newPos = (*(CVector3*)&start.getOrigin());
			newPos.y += addPos.y;

			end.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));

			PhysicsWorld().ConvexSweepTest((const btConvexShape*)collider.GetBody(), start, end, callback);
		}
		if (callback.isHit) {
			//当たった。
			float t = fabsf(acosf(callback.hitNormal.Dot(CVector3::Up)));
			if (t < CMath::PI * 0.3f) {
				//地面。
				CVector3 Circle;
				float x = 0.0f;
				float offset = 0.0f;	//押し戻す量。
				Circle = CVector3::Zero;

				Circle = position;
				Circle.y = callback.hitPos.y;//円の中心
				CVector3 v;
				v.Subtract(Circle, callback.hitPos);
				x = v.Length();//物体の角とプレイヤーの間の横幅の距離が求まる。

				offset = sqrt(max(0.0f, radius*radius - x*x));//yの平方根を求める。

				moveSpeed.y = 0.0f;
				isJump = false;
				nextPosition.y = callback.hitPos.y + offset - radius;
			}
		}
	}
	position = nextPosition;
}
/*!
* @brief	アニメーション再生。
*/
void EnemyTest::PlayAnimation(AnimationNo animNo)
{
	if (currentAnimSetNo != animNo) {
		//別のアニメーション
		animation.PlayAnimation(animNo, 0.3f);
		currentAnimSetNo = animNo;
	}
}
/*!
* @brief	アニメーションコントロール。
*/
void EnemyTest::AnimationControl()
{
	animation.Update(GameTime().GetFrameDeltaTime());
	if (Pad(0).IsPress(enButtonB)) {
		PlayAnimation(AnimationAttack);
	}else{
		if (state == enStateRun) {
			//歩きアニメーション。
			PlayAnimation(AnimationWalk);
		}
		else if (state == enStateStand) {
			//立ちアニメーションを流す。
			PlayAnimation(AnimationStand);
		}
	}
}
/*!
* @brief	描画。
*/
void EnemyTest::Render(CRenderContext& renderContext)
{
	if (state != enState_RideOnCar) {
		//車に乗っているときは非表示にする。
		skinModel.Draw(renderContext, g_camera->GetCamera().GetViewMatrix(), g_camera->GetCamera().GetProjectionMatrix());
	}
}
