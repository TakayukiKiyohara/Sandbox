/*!
 *@brief	プレイヤーのMPバー。
 */

#pragma once

class PlayerMPBar : public IGameObject{
public:
	PlayerMPBar();
	~PlayerMPBar();
	void Start() override;
	void Update() override;
	void PostRender(CRenderContext& renderContext) override;
private:
	CSprite  m_mpBar;			//HPバー。
	CSprite  m_mpGauge;			//HPゲージ。
	CTexture m_mpBarTex;		//HPバーのテクスチャ。
	CTexture m_mpGaugeTex;		//HPゲージのテクスチャ。
};