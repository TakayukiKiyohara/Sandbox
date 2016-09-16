/*!
 * @brief	�v�������_�[
 */

#ifndef _TKPRERENDER_H_
#define _TKPRERENDER_H_

#include "tkEngine/graphics/tkGraphicsConfig.h"
#include "tkEngine/graphics/preRender/tkIDMap.h"
#include "tkEngine/graphics/preRender/tkShadowMap.h"
#include "tkEngine/graphics/preRender/tkReflectionMap.h"

namespace tkEngine{
	/*!
	 * @brief	�v�������_�����O�B
	 */
	class CPreRender : Noncopyable{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CPreRender();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CPreRender();
		/*!
		* @brief	�J��
		*/
		void Release();
		/*!
		 * @brief	�쐬�B
		 *@param[in]	config	�R���t�B�O�B
		 */
		void Create( const SGraphicsConfig& config );
		/*!
		 * @brief	�`��B
		 */
		void Render( CRenderContext& context )
		{
			m_idMap.RenderToIDMap(context);
			m_shadowMap.RenderToShadowMap(context);
			m_reflectionMap.Render(context);
		}
		/*!
		* @brief	�X�V�B
		*/
		void Update()
		{
			m_shadowMap.Update();
		}
		/*!
		* @brief	IDMap�̎擾�B
		*/
		CIDMap& GetIDMap()
		{
			return m_idMap;
		}
		/*!
		* @brief	�V���h�E�}�b�v�̎擾�B
		*/
		CShadowMap& GetShadowMap()
		{
			return m_shadowMap;
		}
		/*!
		* @brief	���t���N�V�����}�b�v�̎擾�B
		*/
		CReflectionMap& GetReflectionMap()
		{
			return m_reflectionMap;
		}
	private:
		SGraphicsConfig	m_config;			//!<�R���t�B�O�B
		CIDMap			m_idMap;			//!<ID�}�b�v
		CShadowMap		m_shadowMap;		//!<�V���h�E�}�b�v�B
		CReflectionMap	m_reflectionMap;	//!<���t���N�V�����}�b�v�B
	};
}
#endif //_TKPRERENDER_H_