/*!
 * @brief	�o�H�T���B
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/AI/tkPathFinding.h"

namespace tkEngine{
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CPathFinding::CPathFinding()
	{
		m_nodes = nullptr;
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	CPathFinding::~CPathFinding()
	{
	}
	/*!
	 * @brief	�n���ꂽ���W�ɍł��߂��m�[�h����������B
	 *@param[in]	pos		����������W�B
	 *@return	�ł��߂��m�[�h�B
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
	 * @brief	�m�[�h�̃��X�g��ݒ�B
	 */
	void CPathFinding::SetNodes( const std::vector<CPathFinding::SNode*>& nodes )
	{
		m_nodes = &nodes;
	}
	/*!
	 * @brief	���[�g�̌����B
	 *@param[out]	root		�������ꂽ���[�g�̊i�[��B
	 *@param[in]	startPos	�J�n���W�B
	 *@param[in]	targetPos	�ڕW���W�B
	 */
	void CPathFinding::FindRoot(std::vector<CPathFinding::SNode*>* root, const CVector3& startPos, const CVector3& targetPos )
	{
		//���[�g���N���A�B
		root->clear();
		//�m�[�h���������B
		for(auto cn : *m_nodes){
			cn->isDone = false;
			cn->moveCost = -1.0f;
			cn->parentNode = nullptr;
			if (!cn->isPossiblePath) {
				//�ʂ�Ȃ��̂ŒT���ς݂ɂ���B
				cn->isDone = true;
			}
		}
		//�J�n�m�[�h���擾����B
		CPathFinding::SNode* startNode = FindNearNode( startPos );
		
		startNode->moveCost = 0.0f;
		while (true) {
			CPathFinding::SNode*  processNode = nullptr;
			//�S�Ẵm�[�h�ɑ΂��Ċm�F�ƃA�b�v�f�[�g���s���B
			for (auto cn : *m_nodes) {
				if (cn->isDone || cn->moveCost < 0.0f) {
					continue;
				}
				//�������̃m�[�h���Ȃ���Ό��݂̃m�[�h�B
				if (!processNode) {
					processNode = cn;
					continue;
				}
				//�K��ς݂łȂ��m�[�h�̂����A��ԏ������R�X�g�̃m�[�h��T���B
				if (cn->moveCost < processNode->moveCost) {
					//����ɃR�X�g���Ⴂ�B
					processNode = cn;
				}
			}
			if (!processNode) {
				//�S�Ẵ`�F�b�N���I������B
				break;
			}
			//�����ς݂ɂ���B
			processNode->isDone = true;
			//�אڂ��Ă���m�[�h�ɃR�X�g��ݒ肷��B
			for (auto ln : processNode->linkNodes) {
				CVector3 diff;
				diff.Subtract(processNode->position, ln->position);
				float moveCost = processNode->moveCost + diff.Length();
				bool needsUpdate = (ln->moveCost < 0) || (ln->moveCost > moveCost);
				if (needsUpdate) {
					//�X�V�̕K�v����B
					ln->moveCost = moveCost;
					ln->parentNode = processNode;
				}
			}
		}
		//���[�g���쐬����B
		//�ڕW�m�[�h���擾����B
		CPathFinding::SNode*  node = FindNearNode(targetPos);
		while (node != startNode) {
			root->push_back(node);
			node = node->parentNode;
		}
		root->push_back(startNode);
		std::reverse(root->begin(), root->end());
	}
}
