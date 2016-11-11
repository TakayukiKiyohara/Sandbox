/*!
 *@brief	プレイヤーのHPバー。
 */

#include "stdafx.h"
#include "HUD/PlayerHPBar.h"
#include "Player/Player.h"

namespace {
	const CVector2 hpGaugeMaxSize = { 245.0f, 31.5f };
	const CVector2 hpBarMaxSize = { 230.5f, 14.2f };
	const CVector2 hpBarPos = { -592.95f, 320.0f };
	const CVector2 hpGaugePos = { -600.0f, 320.0f };
}
PlayerHPBar::PlayerHPBar()
{
}
PlayerHPBar::~PlayerHPBar()
{
}
void PlayerHPBar::Start()
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
void PlayerHPBar::Update()
{
	//プレイヤーの残HPに応じてＨＰバーのサイズを変える。
	
	float hpRate = (float)g_player->GetHP() / (float)g_player->GetMaxHP();
	CVector2 size = hpBarMaxSize;
	size.x *= hpRate;
	m_hpBar.SetSize(size);
	switch (m_state) {
	case enState_DamageWait:
		m_timer += GameTime().GetFrameDeltaTime();
		if (m_timer > 0.5f) {
			//ダメージ演出開始。
			m_state = enState_Damage;
			m_timer = 0.0f;
		}
		break;
	case enState_Damage: {
		CVector2 sizeHPBack = m_hpBarBack.GetSize();
		sizeHPBack.x -= 1.5f;
		if (sizeHPBack.x < size.x) {
			m_state = enState_Normal;
			sizeHPBack.x = size.x;
		}
		m_hpBarBack.SetSize(sizeHPBack);
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
void PlayerHPBar::PostRender(CRenderContext& renderContext)
{
	m_hpGauge.Draw(renderContext);
	m_hpBarBack.Draw(renderContext);
	m_hpBar.Draw(renderContext);
}
