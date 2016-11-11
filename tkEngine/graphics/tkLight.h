/*!
 *@brief	ライト
 */

#ifndef _TKLIGHT_H_
#define _TKLIGHT_H_

namespace tkEngine {
	/*!
	 *@brief	ライト
	 */
	class CLight : Noncopyable{
	public:
		static const int NUM_DIFFUSE_LIGHT = 4;
	public:
		CLight()
		{
			for( int i = 0; i < NUM_DIFFUSE_LIGHT; i++ ){
				m_diffuseLightDirection[i] = CVector3::AxisX;
				m_diffuseLightColor[i].Set( 0.0f, 0.0f, 0.0f, 0.0f );
			}
			m_ambientLight.Set( 0.1f, 0.1f, 0.1f );
			m_limLightDir.Set(0.0f, 0.0f, 0.0f);
			m_limLightColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
			m_pointLightPosition.Set(0.0f, 0.0f, 0.0f, 0.0f);
			m_pointLightColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		}
		/*!
		 *@brief	ディフューズライトの向きを設定。
		 *@param[in]	no				ライトの番号。
		 *@param[in]	lightDirection	ライトの方向。
		 */
		void SetDiffuseLightDirection( int no, const CVector3 lightDirection )
		{
			TK_ASSERT( no < NUM_DIFFUSE_LIGHT, "error");
			m_diffuseLightDirection[no] = lightDirection;
		}
		/*!
		 *@brief	ディフューズライトの向きを取得。
		 *@param[in]	no		ライトの番号。
		 *@return	ライトの向き。
		 */
		const CVector3& GetDiffuseLightDirection( int no ) const
		{
			return m_diffuseLightDirection[no];
		}
		/*!
		 *@brief	ディフューズライトのカラーを設定。
		 *@param[in]	no			ライトの番号。
		 *@param[in]	lightColor	ライトのカラー。
		 */
		void SetDiffuseLightColor( int no, const CVector4& lightColor )
		{
			TK_ASSERT( no < NUM_DIFFUSE_LIGHT, "error" );
			m_diffuseLightColor[no] = lightColor;
		}
		/*!
		 *@brief	ディフューズライトのカラーを取得。
		 */
		const CVector4& GetDiffuseLightColor( int no ) const
		{
			TK_ASSERT( no < NUM_DIFFUSE_LIGHT, "error" );
			return m_diffuseLightColor[no];
		}
		/*!
		 *@brief	環境光を取得。
		 */
		const CVector3& GetAmbientLight() const
		{
			return m_ambientLight;
		}
		/*!
		 *@brief	環境光を設定。
		 */
		void SetAmbinetLight( const CVector3& ambient )
		{
			m_ambientLight = ambient;
		}
		/*!
		*@brief	リムライトの方向を設定。
		*/
		void SetLimLightDirection(const CVector3& dir)
		{
			m_limLightDir = dir;
		}
		/*!
		*@brief	リムライトの方向を取得。
		*/
		const CVector3& GetLimLightDirection() const
		{
			return m_limLightDir;
		}
		/*!
		*@brief	リムライトの色を設定。
		*/
		void SetLimLightColor(const CVector4& color)
		{
			m_limLightColor = color;
		}
		/*!
		*@brief	リムライトの色を取得。
		*/
		const CVector4& GetLimLightColor() const
		{
			return m_limLightColor;
		}
		/*!
		*@brief	ポイントライトの位置を設定。
		*/
		void SetPointLightPosition(const CVector3& pos)
		{
			m_pointLightPosition.Set(pos.x, pos.y, pos.z, 1.0f);
		}
		/*!
		*@brief	ポイントライトの色を設定
		*/
		void SetPointLightColor(const CVector4& col)
		{
			m_pointLightColor = col;
		}
	private:
		CVector3		m_diffuseLightDirection[NUM_DIFFUSE_LIGHT];		//!<ディフューズライトの向き。
		CVector4		m_diffuseLightColor[NUM_DIFFUSE_LIGHT];			//!<ディフューズライトのカラー(x:R成分、y:G成分、z:B成分、w:スペキュラの強さ)。
		CVector3		m_limLightDir;									//!<リムライトの方向。
		CVector4		m_limLightColor;								//!<リムライトの色。
		CVector4		m_pointLightPosition;							//!<点光源の位置。
		CVector4		m_pointLightColor;								//!<点光源の色。
		CVector3		m_ambientLight;									//!<アンビエントライト。
	};
}
#endif // _TKLIGHT_H_