/*!
 * @brief	CSkinModelData�̃��\�[�X�Ǘ��B
 */
#pragma once

namespace tkEngine{
	class CSkinModelDataHandle;
	class CSkinModel;
	typedef std::shared_ptr<CSkinModelData> CSkinModelDataPtr;
	/*!
	 * @brief	CSkinModelData�̃��\�[�X�Ǘ��B
	 */
	class CSkinModelDataResources{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CSkinModelDataResources();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CSkinModelDataResources();
		/*!
		 * @brief	���[�h�B
		 *@param[out]	skinModelDataHandle	�X�L�����f���f�[�^�̃n���h���̊i�[��B
		 *@param[in]	modelPath			���f���̃t�@�C���p�X�B
		 *@param[in]	anim				�A�j���[�V�����B
		 *@param[in]	isInstancing		�C���X�^���V���O���f���H
		 *@param[in]	numInstance			�C���X�^���X�̐��B
		 */
		void Load(
			CSkinModelDataHandle& skinModelDataHandle,
			const char* modelPath,
			CAnimation* anim,
			bool isInstancing = false,
			int numInstance = 1
		);
		/*!
		* @brief	�X�V�B
		*/
		void Update();
	private:
		typedef std::map<int, CSkinModelDataPtr >	CSkinModelDataMap;
		typedef std::list<CSkinModelDataPtr>		CSkinModelDataList;
		CSkinModelDataMap	m_skinModelDataMap;
		CSkinModelDataList	m_instancingModelDataList;
	};
}