#pragma once

#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/graphics/tkTexture.h"
#include "tkEngine/Physics/tkSphereCollider.h"
#include "tkEngine/Physics/tkRigidBody.h"
#include "tkEngine/character/tkCharacterController.h"
#include "AnimationEventController.h"

namespace tkEngine{
	class CParticleEmitter;
}


/*!
 * @brief	プレイヤー。
 */
class Player : public IGameObject {
private:
	static const int NUM_BATTLE_SEAT = 8;		//シートの数。
public:
	enum AnimationNo {
		AnimationInvalid = -1,
		AnimationStand,		//立ち。
		AnimationWalk,		//歩き。
		AnimationRun,		//走り。
		AnimationJump,		//ジャンプ。
		AnimationAttack_Start,
		AnimationAttack_00 = AnimationAttack_Start,	//攻撃00。
		AnimationAttack_01,	//攻撃01。
		AnimationAttack_02,	//攻撃02。
		AnimationAttack_End = AnimationAttack_02,
		AnimationDamage,	//ダメージ。
		AnimationDeath,		//死亡。
		NumAnimation,		//アニメーションの数。
	};
	//戦闘で使用するシート
	struct SBattleSeat {
		bool		isUse;			//使用中フラグ。
		int			seatNo;			//シート番号。
		CVector3	localPosition;	//ローカル座標。
		CVector3	position;		//ワールド座標。
	};
	enum EnState {
		enStateRun,			//走っている。
		enStateStand,		//立ち止まっている。
		enState_Attack,		//攻撃。
		enState_Damage,		//ダメージを受けている。
		enState_Dead,		//死亡。
	};
	bool					isUpdateAnim;		//

	Player() :
		position(CVector3::Zero),
		isUpdateAnim(false)
	{
		memset(battleSeats, 0, sizeof(battleSeats));
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
	/*!
	* @brief	ポイントライトの位置を取得。
	*/
	const CVector3& GetPointLightPosition() const
	{
		return pointLightPosition;
	}
	/*!
	* @brief	ポイントライトの色を取得。
	*/
	const CVector4& GetPointLightColor() const
	{
		return pointLightColor;
	}
	/*!
	* @brief	未使用のシートを検索。
	*@param[in]	pos		未使用のシートの中からこの座標に一番近いシートを返します。
	*@return		未使用のシートがない場合はNULLが返ってくる。
	*/
	SBattleSeat* FindUnuseSeat(const CVector3& pos) ;
	/*!
	* @brief	骨のワールド行列を検索
	*@details
	* 名前で検索を行っているため、遅いです。頻繁に呼ばないように。
	*@param[in]	boneName	骨の名前。
	*@return 見つかったら骨のワールド行列のアドレスを返す。見つからなかった場合はNULLを返す。
	*/
	CMatrix* FindBoneWorldMatrix(const char* boneName)
	{
		return skinModel.FindBoneWorldMatrix(boneName);
	}
	/*!
	* @brief	HPの取得。
	*/
	int GetHP() const
	{
		return hp;
	}
	/*!
	* @brief	最大HPの取得。
	*/
	int GetMaxHP() const
	{
		return maxHP;
	}
	/*!
	* @brief	MPの取得。
	*/
	float GetMP() const
	{
		return mp;
	}
	/*!
	* @brief	最大MPの取得。
	*/
	float GetMaxMP() const
	{
		return maxMP;
	}
private:
	/*!
	* @brief	状態切り替え。
	*/
	void ChangeState(EnState nextState);
	/*!
	* @brief	ヤラレ処理。
	*/
	void Damage();
	/*!
	* @brief	ポイントライトの位置を更新。
	*/
	void UpdatePointLightPosition();
	/*!
	* @brief	アニメーションコントロール。
	*/
	void AnimationControl();
	/*!
	* @brief	アニメーション再生。
	*/
	void PlayAnimation(AnimationNo animNo, float interpolateTime);
	/*!
	* @brief	バトルで使用するシートを初期化。
	*/
	void InitBattleSeats();
	/*!
	* @brief	バトルで使用するシートを更新。
	*/
	void UpdateBattleSeats();

	void UpdateStateMachine();
	/*!
	* @brief	マジックポイントを使用する。
	*/
	void UseMagicPoint(float useMp)
	{
		mp = max(0, mp - useMp);
	}
	/*!
	* @brief	マジックポイントの回復。
	*/
	void RecoverMagicPoint(float recoverMp)
	{
		mp = min(maxMP, mp + recoverMp);
	}
private:
	CSkinModelDataHandle	skinModelData;
	CSkinModel				skinModel;			//スキンモデル。
	CAnimation				animation;			//アニメーション。
	CLight					light;				//ライト。
	CTexture				normalMap;			//法線マップ。
	CTexture				specMap;			//スペキュラマップ。
	CTexture				weaponNormalMap;	//武器の法線マップ。
	CTexture				weaponSpecMap;		//武器のスペキュラマップ。
	CVector3				position;			//座標。
	CQuaternion				rotation;			//回転
	CVector3				toLightPos;			//
	CVector3				pointLightPosition;	//ポイントライトの位置。
	CVector4				pointLightColor;	//ポイントライトのカラー。
	CVector3				toLampLocalPos;		//ランプのローカル座標。
	bool					isApplyDamageTrigger = false;
	EnState					state;				//状態。
	EnState					lastFrameState;		//前のフレームの状態。
	bool					isPointLightOn;		//ポイントライトのスイッチ。
	CRigidBody				rigidBody;			//剛体。
	CCharacterController	characterController;	//キャラクタコントローラ。
	SBattleSeat				battleSeats[NUM_BATTLE_SEAT];	//シート。
	AnimationNo				reqAttackAnimNo;		//再生のリクエストを出している攻撃モーション番号。
	AnimationNo				nextAttackAnimNo;		//次の攻撃モーション番号。
	AnimationEventController	animationEventController;	//アニメーションイベントコントローラ。
	std::list<CParticleEmitter*>	particleEmitterList;
	int						hp =  100;					//ヒットポイント。
	int						maxHP = 100;				//最大ヒットポイント。
	float					mp = 100.0f;				//マジックポイント。
	float					maxMP = 100.0f;				//最大マジックポイント。
	float					radius = 0.0f;
	float					height = 0.0f;
	float					timer = 0.0f;				//タイマー
	float					magicPointRecoverTimer = 0.0f;		//マジックポイントの回復タイマー。
};