/*!
 * @brief	CSkinModelDataのリソース管理。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/resource/tkSkinModelDataResources.h"
#include "tkEngine/resource/tkSkinModelDataHandle.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CSkinModelDataResources::CSkinModelDataResources()
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CSkinModelDataResources::~CSkinModelDataResources()
	{
	}
	/*!
	 * @brief	ロード。
	 *@param[out]	skinModelDataHandle	スキンモデルデータのハンドルの格納先。
	 *@param[in]	modelPath		モデルのファイルパス。
	 *@param[in]	anim			アニメーション。
	 *@param[in]	isInstancing	インスタンシングモデル？
	 *@param[in]	numInstance			インスタンスの数。
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
			//インスシングモデルはモデルデータの使い回しは無理。
			CSkinModelDataPtr newSkinModelData(new CSkinModelData);
			newSkinModelData->LoadModelData(modelPath, anim);
			//インスタンス描画用のデータを作成。
			tkEngine::SVertexElement vertexElement[] = {
				{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },  // WORLD 1行目
				{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },  // WORLD 2行目
				{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },  // WORLD 3行目
				{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },  // WORLD 4行目
				D3DDECL_END()
			};
			newSkinModelData->CreateInstancingDrawData(numInstance, vertexElement);
			m_instancingModelDataList.push_back(newSkinModelData);
			skinModelDataHandle.Init(newSkinModelData, anim, false);
		}else{
			//モデル名をハッシュ値に変換する。
			int hash = CUtil::MakeHash(modelPath);
			auto& it = m_skinModelDataMap.find(hash);
			if (it == m_skinModelDataMap.end()) {
				//新規
				CSkinModelDataPtr newSkinModelData(new CSkinModelData);
				newSkinModelData->LoadModelData(modelPath, anim);
				m_skinModelDataMap.insert(std::pair<int, CSkinModelDataPtr>(hash, newSkinModelData));
				skinModelDataHandle.Init(newSkinModelData, anim, false);
			}
			else {
				//既存のモデルなので使い回す。
				skinModelDataHandle.Init(it->second, anim, true);
			}
		}
	}
	/*!
	* @brief	更新。
	*/
	void CSkinModelDataResources::Update()
	{
		//参照カウンタが1になっているCSkinModelDataをガベージコレクト。
		std::vector<CSkinModelDataMap::iterator>	deleteItList;
		for (
			CSkinModelDataMap::iterator it = m_skinModelDataMap.begin();
			it != m_skinModelDataMap.end();
			it++
		) {
			if (it->second.unique()) {
				//こいつを参照しているモデルはもういない。
				deleteItList.push_back(it);
			}
		}
		for (auto& delIt : deleteItList) {
			m_skinModelDataMap.erase(delIt);
		}
		//続いてインスタンシングモデル。
		for (
			CSkinModelDataList::iterator it = m_instancingModelDataList.begin();
			it != m_instancingModelDataList.end();
			) {
			if (it->unique()) {
				//こいつを参照しているモデルはもういない。
				it = m_instancingModelDataList.erase(it);
			}
			else {
				it++;
			}
		}
	}
}