/*!
 * @brief	ID�}�b�v
 */

#ifndef _TKIDMAP_H_
#define _TKIDMAP_H_

#include "tkEngine/graphics/tkRenderTarget.h"

namespace tkEngine{
	/*!
	 * @brief	IDMap���f���B
	 */
	class CIDMapModel {
	public:
		CIDMapModel();
		~CIDMapModel();
		/*!
		 * @brief	�v���~�e�B�u����쐬�B
		 */
		void Create(CPrimitive* prim);
		/*!
		* @brief	���[���h�r���[�v���W�F�N�V�����s���ݒ�B
		*/
		void SetWVPMatrix(const CMatrix& mWVP)
		{
			m_mWVP = mWVP;
		}
		/*!
		* @brief	�`��B
		*@param[in]	renderContext	�����_�[�R���e�L�X�g�B
		*@param[in]	pEffect			�G�t�F�N�g
		*/
		void Render(CRenderContext& renderContext, CEffect* pEffect );
	private:
		CPrimitive*		m_prim;		//!<�v���~�e�B�u�B
		CMatrix			m_mWVP;		//!<���[���h�r���[�v���W�F�N�V�����s��B
	};
	class CIDMap : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^
		 */
		CIDMap();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CIDMap();
		/*!
		 * @brief	ID�}�b�v�̍쐬�B
		 */
		void Create(int w, int h);
		/*!
		 * @brief	�J���B
		 */
		void Release();
		/*!
		 * @brief	���f�����G���g���[�B
		 */
		void Entry( CIDMapModel* model );
		/*!
		 * @brief	ID�}�b�v�ɏ�������
		 *@param[in]	renderContext	�����_�����O�R���e�L�X�g�B
		 */
		void RenderToIDMap( CRenderContext& renderContext );
		/*!
		* @brief	ID�}�b�v�̃e�N�X�`�����擾�B
		*/
		const CTexture* GetTexture() const
		{
			return m_idMapRT.GetTexture();
		}
		/*!
		* @brief	ID�}�b�v�̕����擾�B
		*/
		int GetWitdh() const
		{
			return m_idMapRT.GetWidth();
		}
		/*!
		* @brief	ID�}�b�v�̍������擾�B
		*/
		int GetHeight() const
		{
			return m_idMapRT.GetHeight();
		}
	private:
		bool						m_isEnable;		//!<�L���H
		CRenderTarget				m_idMapRT;		//!<ID�}�b�v���������ރ����_�����O�^�[�Q�b�g�B
		std::vector<CIDMapModel*>	m_idMapModels;	//!<ID�}�b�v�̕`����s�����f���̃��X�g�B
		CEffect*					m_pIDMapEffect;	//!<
	};
}
#endif // _TKIDMAP_H_
