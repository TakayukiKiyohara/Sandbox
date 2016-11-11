/*!
 *@brief	�v���C���[��HP�o�[�B
 */

#pragma once

class PlayerHPBar : public IGameObject{
public:
	PlayerHPBar();
	~PlayerHPBar();
	void Start() override;
	void Update() override;
	void PostRender(CRenderContext& renderContext) override;
private:
	enum EnState {
		enState_Normal,
		enState_DamageWait,		//�_���[�W���o�J�n�̑҂��B
		enState_Damage,			//�_���[�W���o�B

	};
	CSprite  m_hpBar;			//HP�o�[�B
	CSprite  m_hpGauge;			//HP�Q�[�W�B
	CSprite  m_hpBarBack;		//HP�o�[�̔w�i�B
	CTexture m_hpBarTex;		//HP�o�[�̃e�N�X�`���B
	CTexture m_hpGaugeTex;		//HP�Q�[�W�̃e�N�X�`���B
	CTexture m_hpBarBackTex;	//HP�o�[�̔w�i�̃e�N�X�`��
	EnState m_state = enState_Normal;
	int m_hpLastFrame = 0;			//�v���C���[��1�t���[���O�̂g�o�G
	float m_timer = 0.0f;		//�^�C�}�[�B
};