/*!
 *@brief	のHPバー。
 */

#include "stdafx.h"
#include "HUD/HPBar.h"
#include "Player/Player.h"

namespace {
	const CVector2 hpGaugeMaxSize = { 245.0f, 31.5f };
	const CVector2 hpBarMaxSize = { 230.5f, 14.2f };
	const CVector2 hpBarPos = { -592.95f, 320.0f };
	const CVector2 hpGaugePos = { -600.0f, 320.0f };
}
HPBar::HPBar()
{
}
HPBar::~HPBar()
{
}
void HPBar::Start()
{
	m_hpBarTex.Load("Assets/sprite/hp.png");
	m_hpGaugeTex.Load("Assets/sprite/hpsp_bar.png");
	m_hpBarBackTex.Load("Assets/sprite/hp_back.png");

	m_hpBar.Init(&m_hpBarTex);
	m_hpGauge.Init(&m_hpGaugeTex);
	m_hpBarBack.Init(&m_hpBarBackTex);

	m_hpGauge.SetSize(hpGaugeMaxSize);
	m_hpBar.SetSize(hpBarMaxSize);
	m_hpBarBack.SetSize(hpBarMaxSize);
	//ピボットは左中央。
	m_hpBar.SetPivot({ 0.0f, 0.5f });
	m_hpGauge.SetPivot({ 0.0f, 0.5f });
	m_hpBarBack.SetPivot({ 0.0f, 0.5f });

	m_hpBar.SetPosition(hpBarPos);
	m_hpBarBack.SetPosition(hpBarPos);
	m_hpGauge.SetPosition(hpGaugePos);

	m_hpLastFrame = g_player->GetHP();
}
void HPBar::Update()
{
	//プレイヤーの残HPに応じてＨＰバーのサイズを変える。
	
	float hpRate = (float)g_player->GetHP() / (float)g_player->GetMaxHP();
	CVector2 size = hpBarMaxSize;
	size.x *= hpRate;
	m_hpBar.SetSize(size);
	if (m_enDamageEffect == enDamageEffect_Enemy) {
		m_hpBarBack.SetSize(size);
	}
	switch (m_state) {
	case enState_DamageWait:
		if (m_enDamageEffect == enDamageEffect_Player) {
			m_timer += GameTime().GetFrameDeltaTime();
			if (m_timer > 0.5f) {
				//ダメージ演出開始。
				m_state = enState_Damage;
				m_timer = 0.0f;
			}
		}
		else if (m_enDamageEffect == enDamageEffect_Enemy) {
			//背景の緑のバーも一気に縮む。
			m_timer = 0.0f;
			m_state = enState_Damage;
		}
		break;
	case enState_Damage: {
		if (m_enDamageEffect == enDamageEffect_Player) {
			//小ダメージをダメージ演出。
			CVector2 sizeHPBack = m_hpBarBack.GetSize();
			sizeHPBack.x -= 1.5f;
			if (sizeHPBack.x < size.x) {
				m_state = enState_Normal;
				sizeHPBack.x = size.x;
			}
			m_hpBarBack.SetSize(sizeHPBack);
		}else if(m_enDamageEffect == enDamageEffect_Enemy) {
			//大ダメージを受けた時のダメージ演出。
			//一定時間HPバーを揺らす。
			m_timer += GameTime().GetFrameDeltaTime();
			CQuaternion rot;
			if (m_timer > 1.0f) {
				//終わり。
				rot = CQuaternion::Identity;
				m_state = enState_Normal;
			}
			else {
				rot.SetRotation(CVector3::AxisZ, sinf(m_timer * 40.0f) * 0.25f);
			}
			m_hpGauge.SetRotation(rot);
			m_hpBarBack.SetRotation(rot);
			m_hpBar.SetRotation(rot);
		}
	}break;
	case enState_Normal:
		if (m_hpLastFrame > g_player->GetHP()) {
			//ダメージを受けた。
			m_state = enState_DamageWait;
			m_timer = 0.0f;
		}
		else if (m_hpLastFrame < g_player->GetHP()) {
			//回復している。
			m_hpBarBack.SetSize(size);
		}
		break;
	}
	m_hpLastFrame = g_player->GetHP();
}
void HPBar::PostRender(CRenderContext& renderContext)
{
	m_hpGauge.Draw(renderContext);
	m_hpBarBack.Draw(renderContext);
	m_hpBar.Draw(renderContext);
}
