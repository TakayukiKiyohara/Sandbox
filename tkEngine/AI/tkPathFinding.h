/*!
 * @brief	経路探索。
 */
#ifndef _TKPATHFINDING_H_
#define _TKPATHFINDING_H_

namespace tkEngine{

	/*!
	 * @brief	経路探索。
	 */
	class CPathFinding{
	public:
		/*!
		 * @brief	ノード。
		 */
		struct SNode{
			std::vector<SNode*> linkNodes;		//!<隣接ノード。
			CVector3 			position;		//!<ノードの座標。
			SNode* 			parentNode;		//!<親ノード。
			float			moveCost;		//!<移動コスト。
			bool			isDone;			//!<探索済みフラグ。
			bool			isPossiblePath;	//!<進行可能？
		};
		/*!
		 * @brief	コンストラクタ。
		 */
		CPathFinding();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPathFinding();
		/*!
		 * @brief	ノードのリストを設定。
		 */
		void SetNodes( const std::vector<SNode*>& nodes );
		/*!
		 * @brief	ルートの検索。
		 *@param[out]	root		検索されたルートの格納先。
		 *@param[in]	startPos	開始座標。
		 *@param[in]	targetPos	目標座標。
		 */
		void FindRoot(std::vector<SNode*>* root, const CVector3& startPos, const CVector3& targetPos );
	private:
		/*!
		* @brief	渡された座標に最も近いノードを検索する。
		*@param[in]	pos		検索する座標。
		*@return	最も近いノード。
		*/
		SNode* FindNearNode(const CVector3& pos) const;
	private:
		const std::vector<SNode*>*	 m_nodes;	//!<ノード。
	};
}

#endif //_TKPATHFINDING_H_