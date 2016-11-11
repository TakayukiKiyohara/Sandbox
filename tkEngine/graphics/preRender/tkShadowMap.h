/*!
 * @brief	シャドウマップ。
 */

#ifndef _TKSHADOWMAP_H_
#define _TKSHADOWMAP_H_

#include "tkEngine/graphics/tkBlur.h"


namespace tkEngine{
	class IShadowCaster;
	class CCamera;
	class CShadowMap : Noncopyable{
	public:
		static const int MAX_SHADOW_MAP = 3;						//!<シャドウマップの最大数。
		//シャドウレシーバー用のパラメータ。
		struct ShadowRecieverParam {
			CMatrix		mLVP[MAX_SHADOW_MAP];
			int			vsmFlag_numShadowMap[4];
		};
		//!<ライトビューの計算の仕方。
		enum EnCalcLightViewFunc {
			enCalcLightViewFunc_PositionDirection,	//ライトの位置と方向で計算する。
			enCalcLightViewFunc_PositionTarget,		//ライトの位置と注視点で計算する。
		};
		/*!
		 * @brief	コンストラクタ。
		 */
		CShadowMap();
		/*!
		 * @brief	デストラクタ。
		 */
		~CShadowMap();
		/*!
		 * @brief	作成。
		 */
		void Create( int w, int h, bool isEnableSoftShadow, int numShadowMap );
		/*!
		 * @brief	開放。
		 */
		void Release();
		/*!
		 * @brief	シャドウキャスターをエントリー。
		 */
		void Entry( IShadowCaster* caster );
		/*!
		 * @brief	シャドウマップに書き込み。
		 *@param[in]	renderContext		レンダリングコンテキスト。
		 */
		void RenderToShadowMap( CRenderContext& renderContext );
		/*!
		 * @brief	シャドウマップのテクスチャを取得。
		 */
		const CTexture* GetTexture(int texNo) const
		{
			TK_ASSERT(texNo < MAX_SHADOW_MAP, "texNo is invalid");
			if (!m_isDisableVSM) {
				return m_blur[texNo].GetTexture();
			}
			return m_shadowMapRT[texNo].GetTexture();
		}
		/*!
		 * @brief	ライトの方向を設定。
		 *@details
		 * ライトビューの計算方法がenCalcLightViewFunc_PositionDirectionの時に有効になる。
		 */
		void SetLightDirection( const CVector3& lightDir )
		{
			m_lightDirection = lightDir;
		}
		/*!
		* @brief	ライトビューの注視点を設定。
		*@details
		*  ライトビューの計算方法がenCalcLightViewFunc_PositionTargetの時に有効になる。
		*/
		void SetLightTarget(const CVector3& lightTarget)
		{
			m_lightTarget = lightTarget;
		}
		/*!
		 * @brief	ライトの位置を設定。
		 */
		void SetLightPosition( const CVector3& lightPosition )
		{
			m_lightPosition = lightPosition;
		}
		/*!
		* @brief	ライトビュープロジェクション行列を取得。
		*/
		const CMatrix& GetLVPMatrix() const
		{
			return m_LVPMatrix[0];
		}
		/*!
		* @brief	遠平面を設定。
		*/
		void SetFar(float Far)
		{
			m_far = Far;
		}
		/*!
		* @brief	遠平面を設定。
		*/
		void SetNear(float Near)
		{
			m_far = Near;
		}
		/*!
		* @brief	更新。
		*/
		void Update();
		/*!
		* @brief	近平面。
		*/
		float GetNear() const
		{
			return m_near;
		}
		/*!
		* @brief	遠平面
		*/
		float GetFar() const
		{
			return m_far;
		}
		/*!
		* @brief	シャドウマップが有効か判定。
		*/
		bool IsEnable() const
		{
			return m_isEnable;
		}
		/*!
		* @brief	カメラを設定。PSMを行うときに使用される。
		*/
		void SetCamera(CCamera* camera)
		{
			m_camera = camera;
		}
		/*!
		* @brief	ライトビューの計算の仕方を設定。
		*/
		void SetCalcLightViewFunc(EnCalcLightViewFunc func)
		{
			m_calcLightViewFunc = func;
		}
		/*!
		* @brief	シャドウレシーバー用のパラメータを取得。
		*/
		const ShadowRecieverParam& GetShadowRecieverParam() const
		{
			return m_shadowRecieverParam;
		}
	private:
		bool								m_isEnable;								//!<有効？
		int									m_numShadowMap;							//!<シャドウマップの枚数。
		std::vector<IShadowCaster*>			m_shadowCaster;							//!<シャドウキャスター。
		CEffect*							m_pShadowMapEffect;						//!<シャドウマップのエフェクト。
		CVector3							m_lightPosition;						//!<ライトの位置。
		CVector3							m_lightDirection;						//!<ライトの向き。
		CVector3							m_lightTarget;							//!<注視点
		CMatrix								m_lvMatrix;
		CMatrix								m_LVPMatrix[MAX_SHADOW_MAP];			//!<ライトビュープロジェクション行列。
		CMatrix								m_projectionMatrix;
		float								m_near;									//!<近平面。
		float								m_far;									//!<遠平面。
		float								m_accpect;
		float								m_shadowAreaW[MAX_SHADOW_MAP];			//!<影を落とす範囲の幅。
		float								m_shadowAreaH[MAX_SHADOW_MAP];			//!<影を落とす範囲の高さ。
		CCamera*							m_camera;								//!<PSMを計算するときに使用するカメラ。
		EnCalcLightViewFunc					m_calcLightViewFunc;					//!<ライトビューの計算方法。
		bool								m_isDisableVSM;							//!<VSMが無効？
		CBlur								m_blur[MAX_SHADOW_MAP];					//!<深度マップの平均値を求めるためのブラー処理。
		CRenderTarget						m_shadowMapRT[MAX_SHADOW_MAP];			//!<シャドウマップを書き込むレンダリングターゲット。
		ShadowRecieverParam					m_shadowRecieverParam;					//!<シャドウレシーバー用のパラメータ。
		
	};
}

#endif //_TKSHADOWMAP_H_