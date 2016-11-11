/*!
 * @brief	アニメーションイベントコントローラ。
 */

#pragma once

#include "AnimationEventMacro.h"

const int ANIMATION_EVENT_MAX = 32;

/*!
 * @brief	アニメーションイベントの種類。
 */
enum EAnimationEventType{
	eAnimationEventType_EmitDamageToEnemyCollision,		//敵に対してダメージを与えるコリジョンを生成。
	eAnimationEventType_EmitDamageToPlayerCollision,	//プレイヤに対してダメージを与えるコリジョンを生成。
	eAnimationEventType_EmitSound,						//Soundの再生。
	eAnimationEventType_Emit3DSound,					//3DSoundの再生。
	eAnimationEventType_Invalid = 0xffffffff,
};
/*!
 * @brief	アニメーションイベント。
 */
struct AnimationEvent{
	AnimationEvent(){}
	AnimationEvent(
		EAnimationEventType type,
		float time,
		float fArg0, float fArg1, float fArg2, float fArg3,
		int iArg0, int iArg1, int iArg2, int iArg3,
		const char* strArg0, const char* strArg1, const char* strArg2, const char* strArg3,
		CVector3 vArg0, CVector3 vArg1, CVector3 vArg2, CVector3 vArg3
		)
	{
		this->eventType = type;
		this->time = time;
		fArg[0] = fArg0;
		fArg[1] = fArg1;
		fArg[2] = fArg2;
		fArg[3] = fArg3;

		iArg[0] = iArg0;
		iArg[1] = iArg1;
		iArg[2] = iArg2;
		iArg[3] = iArg3;

		strArg[0] = strArg0;
		strArg[1] = strArg1;
		strArg[2] = strArg2;
		strArg[3] = strArg3;

		vArg[0] = vArg0;
		vArg[1] = vArg1;
		vArg[2] = vArg2;
		vArg[3] = vArg3;
	}
	EAnimationEventType eventType;			//イベントのタイプ。
	float 				time;				//イベントが発生する時間。該当するアニメーションが再生されて経過した時間です。単位は秒。
	float				fArg[4];			//浮動小数点型の引数。
	int					iArg[4];			//整数型の引数。
	const char*			strArg[4];			//文字列引数。
	CVector3			vArg[4];			//ベクトル型の引数。
};
/*!
 * @brief	アニメーショングループ。ひとつのアニメーションで発生するイベントをグループ化したもの。
 */
struct AnimationEventGroup{
	AnimationEvent event[ANIMATION_EVENT_MAX];
};

/*!
 * @brief	アニメーションイベントコントローラ。
 */
class AnimationEventController{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	AnimationEventController();
	/*!
	 * @brief	デストラクタ。
	 */
	~AnimationEventController();
	/*!
	 * @brief	初期化。
	 *@code
	 
	 *@endcode
	 *@param[in]	skinModel		スキンモデル。
	 *@param[in]	animation		アニメーション。
	 *@param[in]	eventGroupTbl	イベントグループのテーブル。
	 *@param[in]	tblSize			イベントグループのテーブルのサイズ。
	 */
	void Init(CSkinModel* skinModel, CAnimation* animation, AnimationEventGroup* eventGroupTbl, int tblSize );
	/*!
	 * @brief	更新。
	 */
	void Update();
private:
	/*!
	 * @brief	アニメーションイベントの発生。
	 */
	void InvokeAnimationEvent(const AnimationEvent& event);
private:
	struct AnimationEventGroupEx{
		AnimationEventGroup eventGroup;
		bool invokeFlags[ANIMATION_EVENT_MAX];	//イベントの発生フラグ。
	};
	int									animNoLastFrame = -1;		//!<一フレーム前のアニメーション番号。
	CAnimation* 						animation = nullptr;		//!<アニメーション。
	std::vector<AnimationEventGroupEx>	eventGroupExTbl;	
	CSkinModel*							skinModel = nullptr;		//!<スキンモデル。
	float								lastFrameAnimTime = 0.0f;	//!<1フレーム前のアニメーションタイム。
	bool								isInited = false;			//!<初期化できたかどうかのフラグ。
};