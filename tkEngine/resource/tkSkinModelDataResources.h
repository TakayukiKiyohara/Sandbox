/*!
 * @brief	CSkinModelDataのリソース管理。
 */
#pragma once

namespace tkEngine{
	class CSkinModelDataHandle;
	class CSkinModel;
	typedef std::shared_ptr<CSkinModelData> CSkinModelDataPtr;
	/*!
	 * @brief	CSkinModelDataのリソース管理。
	 */
	class CSkinModelDataResources{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CSkinModelDataResources();
		/*!
		 * @brief	デストラクタ。
		 */
		~CSkinModelDataResources();
		/*!
		 * @brief	ロード。
		 *@param[out]	skinModelDataHandle	スキンモデルデータのハンドルの格納先。
		 *@param[in]	modelPath			モデルのファイルパス。
		 *@param[in]	anim				アニメーション。
		 *@param[in]	isInstancing		インスタンシングモデル？
		 *@param[in]	numInstance			インスタンスの数。
		 */
		void Load(
			CSkinModelDataHandle& skinModelDataHandle,
			const char* modelPath,
			CAnimation* anim,
			bool isInstancing = false,
			int numInstance = 1
		);
		/*!
		* @brief	更新。
		*/
		void Update();
	private:
		typedef std::map<int, CSkinModelDataPtr >	CSkinModelDataMap;
		typedef std::list<CSkinModelDataPtr>		CSkinModelDataList;
		CSkinModelDataMap	m_skinModelDataMap;
		CSkinModelDataList	m_instancingModelDataList;
	};
}