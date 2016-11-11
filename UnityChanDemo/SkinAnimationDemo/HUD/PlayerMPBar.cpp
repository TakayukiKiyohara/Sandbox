/*!
 *@brief	プレイヤーのMPバー。
 */

#include "stdafx.h"
#include "HUD/PlayerMPBar.h"
#include "Player/Player.h"

namespace {
	const CVector2 mpGaugeMaxSize = { 245.0f, 31.5f };
	const CVector2 mpBarMaxSize = { 230.5f, 14.2f };
	const CVector2 mpBarPos = { -592.95f, 278.0f };
	const CVector2 mpGaugePos = { -600.0f, 278.0f };
}
PlayerMPBar::PlayerMPBar()
{
}
PlayerMPBar::~PlayerMPBar()
{
}
void PlayerMPBar::Start()
{
	m_mpBarTex.Load("Assets/sprite/mp.png");
	m_mpGaugeTex.Load("Assets/sprite/hpsp_bar.png");

	m_mpBar.Init(&m_mpBarTex);
	m_mpGauge.Init(&m_mpGaugeTex);

	m_mpGauge.SetSize(mpGaugeMaxSize);
	m_mpBar.SetSize(mpBarMaxSize);
	//ピボットは左中央。
	m_mpBar.SetPivot({ 0.0f, 0.5f });
	m_mpGauge.SetPivot({ 0.0f, 0.5f });

	m_mpBar.SetPosition(mpBarPos);
	m_mpGauge.SetPosition(mpGaugePos);

}
void PlayerMPBar::Update()
{
	//プレイヤーの残HPに応じてMPバーのサイズを変える。
	
	float mpRate = g_player->GetMP() / g_player->GetMaxMP();
	CVector2 size = mpBarMaxSize;
	size.x *= mpRate;
	m_mpBar.SetSize(size);
}
void PlayerMPBar::PostRender(CRenderContext& renderContext)
{
	m_mpGauge.Draw(renderContext);
	m_mpBar.Draw(renderContext);
}
