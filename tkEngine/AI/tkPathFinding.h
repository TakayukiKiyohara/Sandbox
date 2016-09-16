/*!
 * @brief	�o�H�T���B
 */
#ifndef _TKPATHFINDING_H_
#define _TKPATHFINDING_H_

namespace tkEngine{

	/*!
	 * @brief	�o�H�T���B
	 */
	class CPathFinding{
	public:
		/*!
		 * @brief	�m�[�h�B
		 */
		struct SNode{
			std::vector<SNode*> linkNodes;		//!<�אڃm�[�h�B
			CVector3 			position;		//!<�m�[�h�̍��W�B
			SNode* 			parentNode;		//!<�e�m�[�h�B
			float			moveCost;		//!<�ړ��R�X�g�B
			bool			isDone;			//!<�T���ς݃t���O�B
			bool			isPossiblePath;	//!<�i�s�\�H
		};
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CPathFinding();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CPathFinding();
		/*!
		 * @brief	�m�[�h�̃��X�g��ݒ�B
		 */
		void SetNodes( const std::vector<SNode*>& nodes );
		/*!
		 * @brief	���[�g�̌����B
		 *@param[out]	root		�������ꂽ���[�g�̊i�[��B
		 *@param[in]	startPos	�J�n���W�B
		 *@param[in]	targetPos	�ڕW���W�B
		 */
		void FindRoot(std::vector<SNode*>* root, const CVector3& startPos, const CVector3& targetPos );
	private:
		/*!
		* @brief	�n���ꂽ���W�ɍł��߂��m�[�h����������B
		*@param[in]	pos		����������W�B
		*@return	�ł��߂��m�[�h�B
		*/
		SNode* FindNearNode(const CVector3& pos) const;
	private:
		const std::vector<SNode*>*	 m_nodes;	//!<�m�[�h�B
	};
}

#endif //_TKPATHFINDING_H_