/*!
 * @brief	CSkinModelData�̃n���h���N���X�B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/resource/tkSkinModelDataHandle.h"

namespace tkEngine{
	void CSkinModelDataHandle::Init(const std::shared_ptr<CSkinModelData>& skinModelData, CAnimation* anim, bool isClone)
	{
		m_originalSkinModelData = skinModelData;
		if(isClone){
			//�N���[�������H
			m_skinModelData.reset( new CSkinModelData );
			m_skinModelData->CloneModelData(*m_originalSkinModelData, anim);
		}
	}
}