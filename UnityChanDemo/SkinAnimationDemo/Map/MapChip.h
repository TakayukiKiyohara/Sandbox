#pragma once
#include "tkEngine/Physics/tkMeshCollider.h"
#include "tkengine/Physics/tkRigidBody.h"

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
	CSkinModelDataHandle				skinModelData;
	CSkinModel							skinModel;			//�X�L�����f���B
	CAnimation							animation;			//�A�j���[�V�����B
	CLight								light;				//���C�g�B
	std::vector<CTexture>				specMapList;			//�X�y�L�����}�b�v�B
	std::vector<CTexture>				normalMapList;			//�@���}�b�v�B
	std::unique_ptr<CMatrix[]>			worldMatrixBuffer;	//���[���h�s��̃o�b�t�@
	std::unique_ptr<CMeshCollider[]>	meshCollider;		//���b�V���R���C�_�[�B
	std::unique_ptr<CRigidBody[]>		rigidBody;			//���́B
	CMatrix*							rootBoneMatrix;		
};

