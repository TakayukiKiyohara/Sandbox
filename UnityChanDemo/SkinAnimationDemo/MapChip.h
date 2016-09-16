#pragma once
//�}�b�v�`�b�v�̔z�u���B
struct SMapChipLocInfo {
	const char* modelName;		//���f���B
	CVector3	pos;			//���W�B
	CQuaternion	rotation;		//��]�B
};
class MapChip : public IGameObject
{
public:
	MapChip();
	~MapChip();
	void Init(const std::vector<SMapChipLocInfo*>& mapChipLocInfoList);
	void Start() override;
	void Update() override;
	void Render(CRenderContext& renderContext) override;
private:
	CSkinModelData					skinModelData;
	CSkinModel						skinModel;			//�X�L�����f���B
	CAnimation						animation;			//�A�j���[�V�����B
	CLight							light;				//���C�g�B
	CTexture						specMap;			//�X�y�L�����}�b�v�B
	std::unique_ptr<CMatrix[]>		worldMatrixBuffer;	//���[���h�s��̃o�b�t�@
};

