/*!
 * @brief	�|�X�g�G�t�F�N�g
 */

#ifndef _TKPOSTEFFECT_H_
#define _TKPOSTEFFECT_H_

#include "tkEngine/graphics/postEffect/tkEdgeRender.h"
#include "tkEngine/graphics/postEffect/tkBloomRender.h"
#include "tkEngine/graphics/postEffect/tkDOF.h"
#include "tkEngine/graphics/postEffect/tkFxaa.h"

namespace tkEngine{
	/*!
	 * @brief	�|�X�g�G�t�F�N�g�B
	 */
	class CPostEffect{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CPostEffect();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CPostEffect();
		/*!
		* @brief	�J���B
		*/
		void Release();
		/*!
		 * @brief
		 */
		void Create( const SGraphicsConfig& config );
		/*!
		* @brief	�`��B
		*/
		void Render(CRenderContext& renderContext);
		/*!
		* @brief	�t���X�N���[���`��B
		*/
		void RenderFullScreen(CRenderContext& renderContext);
		/*!
		* @brief	DOF���擾�B
		*/
		CDof& GetDof()
		{
			return m_dof;
		}
	private:
		CEdgeRender		m_edgeRender;		//!<�G�b�W�`��B
		CBloomRender	m_bloomRender;		//!<Bloom�B
		CDof			m_dof;				//!<��ʊE�[�x
		CFxaa			m_fxaa;
		CPrimitive		m_fullscreenRenderPrim;	//!<�t���X�N���[���������_�����O���邽�߂̃v���~�e�B�u�B
	};
}

#endif // _TKPOSTEFFECT_H_
