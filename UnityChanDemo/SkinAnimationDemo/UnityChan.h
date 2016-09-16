#pragma once

#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"

/*!
 * @brief	ユニティちゃん
 */
class UnityChan : public IGameObject {
private:
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//立ち。
		AnimationWalk,		//歩き。
		AnimationRun,		//走り。
		AnimationJump,		//ジャンプ。
	};
public:
	enum EnState {
		enStateRun,			//走っている。
		enStateStand,		//立ち止まっている。
		enState_RideOnCar,	//車に乗っている
	};
	bool					isUpdateAnim;		//

	UnityChan() :
		position(CVector3::Zero),
		isUpdateAnim(false)
	{

	}
	void Start() override ;
	void Update() override ;
	void Render( CRenderContext& renderContext ) override;
	void SetPosition(const CVector3& position)
	{
		this->position = position;
	}
	const CVector3& GetPosition() const
	{
		return position;
	}
	/*!
	* @brief	状態を取得。
	*/
	EnState GetState() const
	{
		return state;
	}
private:
	/*!
	* @brief	衝突検出と解決。
	*@param[in]	nextPosition		次の座標。
	*/
	void CollisionDetectAndResolve( const CVector3& nextPosition );
	/*!
	* @brief	アニメーションコントロール。
	*/
	void AnimationControl();
	/*!
	* @brief	アニメーション再生。
	*/
	void PlayAnimation(AnimationNo animNo);
private:
	
	static CSkinModelData*	orgSkinModelData;		//スキンモデルデータ。
	CSkinModelData			skinModelData;
	CSkinModel				skinModel;			//スキンモデル。
	CAnimation				animation;			//アニメーション。
	CLight					light;				//ライト。
	AnimationNo				currentAnimSetNo;
	CTexture				normalMap;			//法線マップ。
	CTexture				specMap;			//スペキュラマップ。
	CVector3				position;			//座標。
	CQuaternion				rotation;			//回転
	CVector3				toLightPos;			//
	CVector3				moveSpeed;			//移動速度。
	EnState					state;				//状態。
	EnState					lastFrameState;		//前のフレームの状態。
	bool					isJump;				//ジャンプ中フラグ。
};