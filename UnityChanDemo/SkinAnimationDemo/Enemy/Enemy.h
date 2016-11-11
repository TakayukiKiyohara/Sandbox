/*!
 * @brief	敵の基底クラス。
 */
 
#pragma once

#include "tkEngine/character/tkCharacterController.h"
#include "AnimationEventController.h"

struct EnemyParam;
class Enemy : public IGameObject{
public:
	enum EnAnimation {
		enAnimStand,
		enAnimWalk,
		enAnimAttack,
		enAnimDamage,
		enAnimDeath,
		enNumAnim,
		enAnimInvalid = 0xffffffff
	};
	Enemy() :
		currentAnimNo(enAnimInvalid),
		moveSpeed(0.0f),
		moveDirection(CVector3::AxisZ)
	{
	}
	virtual ~Enemy()
	{
	}
	virtual void Init(const char* modelPath, CVector3 pos, CQuaternion rotation);
	void Update() override;
	void PlayAnimation(EnAnimation animNo)
	{
		if(currentAnimNo != animNo)
		{
			animation.PlayAnimation(animNo, 0.3f);
			currentAnimNo = animNo;
		}
	}
	//同じアニメーションが再生されていても気にせずにでも最初から流す。
	void PlayAnimationForce(EnAnimation animNo)
	{
		animation.PlayAnimation(animNo, 0.3f);
		currentAnimNo = animNo;
	}
	void SetMoveSpeed(float moveSpeed)
	{
		this->moveSpeed = moveSpeed;
	}
	const CVector3& GetMoveDirection() const
	{
		return moveDirection;
	}
	void SetMoveDirection( const CVector3& dir )
	{
		moveDirection = dir;
	}
	void SetDirection(const CVector3& dir)
	{
		direction = dir;
	}
	const CVector3& GetPosition() const
	{
		return position;
	}
	bool IsPlayAnimation() const
	{
		return animation.IsPlay();
	}
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
	* @brief	敵のパラメータを取得。
	*/
	const EnemyParam* GetEnemyParam() const
	{
		return enemyParam;
	}
	/*!
	* @brief	死亡したことを通知。
	*/
	void NotifyDead();
protected:
	CSkinModelDataHandle			skinModelData;
	CSkinModel						skinModel;			//スキンモデル。
	CAnimation						animation;			//アニメーション。
	CLight							light;				//ライト。
	CVector3						position;			//位置
	CQuaternion						rotation;			//回転。
	EnAnimation						currentAnimNo;		//現在のアニメーション番号。
	float							moveSpeed;			//移動速度。
	CVector3						moveDirection;		//進行方向。
	CVector3						direction;			//向き。
	int								hp;					//体力。
	std::vector<CTexture>			specMapList;		//スペキュラマップ。
	std::vector<CTexture>			normalMapList;		//法線マップ。
	const EnemyParam*				enemyParam;			//敵のパラメータ。
	CVector3						initPosition;			//初期位置。
	AnimationEventController		animationEventController;	//アニメーションイベントコントローラー。
	CCharacterController			characterController;	//キャラクタコントローラ。
	float							height;
	float							radius;
	float							timer = 0.0f;
};