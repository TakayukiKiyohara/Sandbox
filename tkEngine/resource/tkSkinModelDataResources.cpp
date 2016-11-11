/*!
 * @brief	CSkinModelData�̃��\�[�X�Ǘ��B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/resource/tkSkinModelDataResources.h"
#include "tkEngine/resource/tkSkinModelDataHandle.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CSkinModelDataResources::CSkinModelDataResources()
	{
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CSkinModelDataResources::~CSkinModelDataResources()
	{
	}
	/*!
	 * @brief	���[�h�B
	 *@param[out]	skinModelDataHandle	�X�L�����f���f�[�^�̃n���h���̊i�[��B
	 *@param[in]	modelPath		���f���̃t�@�C���p�X�B
	 *@param[in]	anim			�A�j���[�V�����B
	 *@param[in]	isInstancing	�C���X�^���V���O���f���H
	 *@param[in]	numInstance			�C���X�^���X�̐��B
	 */
	void CSkinModelDataResources::Load(
		CSkinModelDataHandle& skinModelDataHandle, 
		const char* modelPath, 
		CAnimation* anim, 
		bool isInstancing,
		int numInstance
	)
	{
		if (isInstancing) {
			//�C���X�V���O���f���̓��f���f�[�^�̎g���񂵂͖����B
			CSkinModelDataPtr newSkinModelData(new CSkinModelData);
			newSkinModelData->LoadModelData(modelPath, anim);
			//�C���X�^���X�`��p�̃f�[�^���쐬�B
			tkEngine::SVertexElement vertexElement[] = {
				{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },  // WORLD 1�s��
				{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },  // WORLD 2�s��
				{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },  // WORLD 3�s��
				{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },  // WORLD 4�s��
				D3DDECL_END()
			};
			newSkinModelData->CreateInstancingDrawData(numInstance, vertexElement);
			m_instancingModelDataList.push_back(newSkinModelData);
			skinModelDataHandle.Init(newSkinModelData, anim, false);
		}else{
			//���f�������n�b�V���l�ɕϊ�����B
			int hash = CUtil::MakeHash(modelPath);
			auto& it = m_skinModelDataMap.find(hash);
			if (it == m_skinModelDataMap.end()) {
				//�V�K
				CSkinModelDataPtr newSkinModelData(new CSkinModelData);
				newSkinModelData->LoadModelData(modelPath, anim);
				m_skinModelDataMap.insert(std::pair<int, CSkinModelDataPtr>(hash, newSkinModelData));
				skinModelDataHandle.Init(newSkinModelData, anim, false);
			}
			else {
				//�����̃��f���Ȃ̂Ŏg���񂷁B
				skinModelDataHandle.Init(it->second, anim, true);
			}
		}
	}
	/*!
	* @brief	�X�V�B
	*/
	void CSkinModelDataResources::Update()
	{
		//�Q�ƃJ�E���^��1�ɂȂ��Ă���CSkinModelData���K�x�[�W�R���N�g�B
		std::vector<CSkinModelDataMap::iterator>	deleteItList;
		for (
			CSkinModelDataMap::iterator it = m_skinModelDataMap.begin();
			it != m_skinModelDataMap.end();
			it++
		) {
			if (it->second.unique()) {
				//�������Q�Ƃ��Ă��郂�f���͂������Ȃ��B
				deleteItList.push_back(it);
			}
		}
		for (auto& delIt : deleteItList) {
			m_skinModelDataMap.erase(delIt);
		}
		//�����ăC���X�^���V���O���f���B
		for (
			CSkinModelDataList::iterator it = m_instancingModelDataList.begin();
			it != m_instancingModelDataList.end();
			) {
			if (it->unique()) {
				//�������Q�Ƃ��Ă��郂�f���͂������Ȃ��B
				it = m_instancingModelDataList.erase(it);
			}
			else {
				it++;
			}
		}
	}
}