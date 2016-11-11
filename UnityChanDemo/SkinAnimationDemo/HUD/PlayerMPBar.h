/*!
 *@brief	�v���C���[��MP�o�[�B
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
	CSprite  m_mpBar;			//HP�o�[�B
	CSprite  m_mpGauge;			//HP�Q�[�W�B
	CTexture m_mpBarTex;		//HP�o�[�̃e�N�X�`���B
	CTexture m_mpGaugeTex;		//HP�Q�[�W�̃e�N�X�`���B
};