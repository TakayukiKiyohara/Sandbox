/*!
 * @brief	経路探索。
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/AI/tkPathFinding.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CPathFinding::CPathFinding()
	{
		m_nodes = nullptr;
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CPathFinding::~CPathFinding()
	{
	}
	/*!
	 * @brief	渡された座標に最も近いノードを検索する。
	 *@param[in]	pos		検索する座標。
	 *@return	最も近いノード。
	 */
	CPathFinding::SNode* CPathFinding::FindNearNode( const CVector3& pos ) const
	{
		float fMin = FLT_MAX;
		CPathFinding::SNode* nearNode = nullptr;
		for(auto n : *m_nodes){
			if (!n->isPossiblePath) {
				continue;
			}
			CVector3 diff;
			diff.Subtract( n->position, pos );
			float L = diff.LengthSq();
			if(L < fMin){
				fMin = L;
				nearNode = n;
			}
		}
		return nearNode;
	}
	/*!
	 * @brief	ノードのリストを設定。
	 */
	void CPathFinding::SetNodes( const std::vector<CPathFinding::SNode*>& nodes )
	{
		m_nodes = &nodes;
	}
	/*!
	 * @brief	ルートの検索。
	 *@param[out]	root		検索されたルートの格納先。
	 *@param[in]	startPos	開始座標。
	 *@param[in]	targetPos	目標座標。
	 */
	void CPathFinding::FindRoot(std::vector<CPathFinding::SNode*>* root, const CVector3& startPos, const CVector3& targetPos )
	{
		//ルートをクリア。
		root->clear();
		//ノードを初期化。
		for(auto cn : *m_nodes){
			cn->isDone = false;
			cn->moveCost = -1.0f;
			cn->parentNode = nullptr;
			if (!cn->isPossiblePath) {
				//通れないので探索済みにする。
				cn->isDone = true;
			}
		}
		//開始ノードを取得する。
		CPathFinding::SNode* startNode = FindNearNode( startPos );
		
		startNode->moveCost = 0.0f;
		while (true) {
			CPathFinding::SNode*  processNode = nullptr;
			//全てのノードに対して確認とアップデートを行う。
			for (auto cn : *m_nodes) {
				if (cn->isDone || cn->moveCost < 0.0f) {
					continue;
				}
				//処理中のノードがなければ現在のノード。
				if (!processNode) {
					processNode = cn;
					continue;
				}
				//訪問済みでないノードのうち、一番小さいコストのノードを探す。
				if (cn->moveCost < processNode->moveCost) {
					//さらにコストが低い。
					processNode = cn;
				}
			}
			if (!processNode) {
				//全てのチェックが終わった。
				break;
			}
			//調査済みにする。
			processNode->isDone = true;
			//隣接しているノードにコストを設定する。
			for (auto ln : processNode->linkNodes) {
				CVector3 diff;
				diff.Subtract(processNode->position, ln->position);
				float moveCost = processNode->moveCost + diff.Length();
				bool needsUpdate = (ln->moveCost < 0) || (ln->moveCost > moveCost);
				if (needsUpdate) {
					//更新の必要あり。
					ln->moveCost = moveCost;
					ln->parentNode = processNode;
				}
			}
		}
		//ルートを作成する。
		//目標ノードを取得する。
		CPathFinding::SNode*  node = FindNearNode(targetPos);
		while (node != startNode) {
			root->push_back(node);
			node = node->parentNode;
		}
		root->push_back(startNode);
		std::reverse(root->begin(), root->end());
	}
}
