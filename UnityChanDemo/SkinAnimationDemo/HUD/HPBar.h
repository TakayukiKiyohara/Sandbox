/*!
 *@brief	HPバー。
 */

#pragma once

class HPBar : public IGameObject{
public:
	HPBar();
	~HPBar();
	void Start() override;
	void Update() override;
	void PostRender(CRenderContext& renderContext) override;
private:
	enum EnDamageEffect {
		enDamageEffect_Player,			//プレイヤー用の演出。
		enDamageEffect_Enemy,			//エネミー用の演出。
	};
	enum EnState {
		enState_Normal,
		enState_DamageWait,		//ダメージ演出開始の待ち。
		enState_Damage,			//ダメージ演出。
	};
	CSprite  m_hpBar;			//HPバー。
	CSprite  m_hpGauge;			//HPゲージ。
	CSprite  m_hpBarBack;		//HPバーの背景。
	CTexture m_hpBarTex;		//HPバーのテクスチャ。
	CTexture m_hpGaugeTex;		//HPゲージのテクスチャ。
	CTexture m_hpBarBackTex;	//HPバーの背景のテクスチャ
	EnState m_state = enState_Normal;
	int m_hpLastFrame = 0;			//プレイヤーの1フレーム前のＨＰ；
	float m_timer = 0.0f;			//タイマー。
	EnDamageEffect m_enDamageEffect = enDamageEffect_Player;	//ダメージ演出の種類。
};