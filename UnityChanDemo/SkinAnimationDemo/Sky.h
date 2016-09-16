#pragma once

class Sky : public IGameObject {
	CSkinModelData	skinModelData;		//�X�L�����f���f�[�^�B
	CSkinModel		skinModel;			//�X�L�����f���B
	CAnimation		animation;			//�A�j���[�V�����B
	CLight			light;				//���C�g�B
	CTexture		normalMap;
	UnityChan*		unity;				//���j�e�B�����
public:
	Sky();
	
	~Sky()
	{

	}
	void Start() override
	{

	}
	void SetUnityChan(UnityChan* unity)
	{
		this->unity = unity;
	}
	void Update() override;
	
	void Render(CRenderContext& renderContext) override;
	
};
