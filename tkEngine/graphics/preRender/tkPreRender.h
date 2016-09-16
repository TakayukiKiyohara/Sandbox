/*!
 * @brief	プリレンダー
 */

#ifndef _TKPRERENDER_H_
#define _TKPRERENDER_H_

#include "tkEngine/graphics/tkGraphicsConfig.h"
#include "tkEngine/graphics/preRender/tkIDMap.h"
#include "tkEngine/graphics/preRender/tkShadowMap.h"
#include "tkEngine/graphics/preRender/tkReflectionMap.h"

namespace tkEngine{
	/*!
	 * @brief	プリレンダリング。
	 */
	class CPreRender : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CPreRender();
		/*!
		 * @brief	デストラクタ。
		 */
		~CPreRender();
		/*!
		* @brief	開放
		*/
		void Release();
		/*!
		 * @brief	作成。
		 *@param[in]	config	コンフィグ。
		 */
		void Create( const SGraphicsConfig& config );
		/*!
		 * @brief	描画。
		 */
		void Render( CRenderContext& context )
		{
			m_idMap.RenderToIDMap(context);
			m_shadowMap.RenderToShadowMap(context);
			m_reflectionMap.Render(context);
		}
		/*!
		* @brief	更新。
		*/
		void Update()
		{
			m_shadowMap.Update();
		}
		/*!
		* @brief	IDMapの取得。
		*/
		CIDMap& GetIDMap()
		{
			return m_idMap;
		}
		/*!
		* @brief	シャドウマップの取得。
		*/
		CShadowMap& GetShadowMap()
		{
			return m_shadowMap;
		}
		/*!
		* @brief	リフレクションマップの取得。
		*/
		CReflectionMap& GetReflectionMap()
		{
			return m_reflectionMap;
		}
	private:
		SGraphicsConfig	m_config;			//!<コンフィグ。
		CIDMap			m_idMap;			//!<IDマップ
		CShadowMap		m_shadowMap;		//!<シャドウマップ。
		CReflectionMap	m_reflectionMap;	//!<リフレクションマップ。
	};
}
#endif //_TKPRERENDER_H_