/*!
 * @brief	�X�L�����f���f�[�^�̃n���h���N���X�B
 */
#pragma once

namespace tkEngine{
	class CSkinModel;
	typedef std::shared_ptr<CSkinModelData> CSkinModelDataPtr;
	/*!
	* @brief	CSkinModelData�̃n���h���N���X�B
	*/
	class CSkinModelDataHandle : Noncopyable{
		std::shared_ptr<CSkinModelData> m_originalSkinModelData;	//!<�I���W�i���̃X�L�����f���f�[�^�B
		std::unique_ptr<CSkinModelData> m_skinModelData;								//!<�X�L�����f���f�[�^�B
	public:
		
		/*!
		 * @brief	CSkinModelData�̐��|�C���^���擾�B
		 */
		CSkinModelData* GetBody()
		{
			if(m_skinModelData){
				return m_skinModelData.get();
			}
			return m_originalSkinModelData.get();
		}
		/*!
		 * @brief	�������B
		 *@details
		 * CSkinModelDataResources����Ă΂�܂��B���[�U�[�͎g�p���Ȃ��悤�ɁB
		 */
		void Init(const std::shared_ptr<CSkinModelData>& skinModelData, CAnimation* anim, bool isClone);
	};
}
