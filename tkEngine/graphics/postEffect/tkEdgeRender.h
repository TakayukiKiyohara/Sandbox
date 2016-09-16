/*!
 * @brief	�G�b�W�`��B
 */

#ifndef _TKEDGERENDER_H_
#define _TKEDGERENDER_H_

namespace tkEngine{
	class CPostEffect;
	/*!
	 * @brief	�G�b�W�`��B
	 */
	class CEdgeRender{
	public:
		CEdgeRender();
		~CEdgeRender();
		void Render( CRenderContext& renderContext, CPostEffect* postEffect );
		void Create(const SGraphicsConfig& config );
	private:
		CEffect*	m_pEffect;
		bool		m_isEnable;
	};
}

#endif // _TKEDGERENDER_H_