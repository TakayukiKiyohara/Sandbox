/*!
 * @brief	カメラ
 */

#ifndef _TKCAMERA_H_
#define _TKCAMERA_H_

namespace tkEngine{
	class CCamera{
	public:
		CCamera();
		~CCamera();
		/*!
		 * @brief	ビュー行列、プロジェクション行列、ビュープロジェクション行列を更新。
		 */
		void Update();
		/*!
		 * @brief	カメラの座標を設定。
		 *@param[in]	pos		カメラの座標。
		 */
		void SetPosition( const CVector3& pos ) 
		{
			m_position = pos;
		}
		/*!
		 * @brief	カメラの座標を取得
		 *@return	カメラの座標。
		 */
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		 * @brief	注視点を設定。
		 *@param[in]	target		注視点。
		 */
		void SetTarget( const CVector3& target )
		{
			m_target = target;
		}
		/*!
		 * @brief	注視点を取得。
		 *@return	注視点。
		 */
		const CVector3& GetTarget() const
		{
			return m_target;
		}
		/*!
		 * @brief	カメラの上方向を設定。
		 */
		void SetUp( const CVector3& up )
		{
			m_up = up;
		}
		/*!
		 * @brief	カメラの上方向を取得
		 */
		const CVector3& GetUp() const
		{
			return m_up;
		}
		/*!
		* @brief	ビュー行列の逆行列を取得。
		*/
		const CMatrix& GetViewMatrixInv() const
		{
			return m_viewMatrixInv;
		}
		/*!
		 * @brief	ビュー行列を取得。
		 */
		const CMatrix& GetViewMatrix() const
		{
			return m_viewMatrix;
		}
		/*!
		 * @brief	プロジェクション行列を取得。
		 */
		const CMatrix& GetProjectionMatrix() const
		{
			return m_projectionMatrix;
		}
		/*!
		 * @brief	ビュープロジェクション行列を取得。
		 */
		const CMatrix& GetViewProjectionMatrix() const
		{
			return m_viewProjectionMatrix;
		}
		/*!
		* @brief	カメラの回転行列を取得。
		*/
		const CMatrix& GetCameraRotation() const
		{
			return m_cameraRotation;
		}
		/*!
		 * @brief	遠平面を設定。
		 */
		void SetFar( float fFar )
		{
			m_far = fFar;
		}
		/*!
		 * @brief	近平面を設定。
		 */
		void SetNear( float fNear )
		{
			m_near = fNear;
		}
		/*!
		 * @brief	遠平面を取得。
		 */
		float GetFar() const
		{
			return m_far;
		}
		/*!
		 * @brief	近平面を取得。
		 */
		float GetNear() const
		{
			return m_near;
		}
		/*!
		* @brief	画角を設定。
		*/
		void SetViewAngle(float viewAngle)
		{
			m_viewAngle = viewAngle;
		}
		/*!
		* @brief	画角を取得。
		*/
		float GetViewAngle() const
		{
			return m_viewAngle;
		}
	private:
		CVector3	m_position;							//!<カメラ位置。
		CVector3	m_up;								//!<カメラの上方向。
		CVector3	m_target;							//!<カメラの中止点。
		CMatrix		m_viewMatrix;						//!<ビュー行列。
		CMatrix		m_projectionMatrix;					//!<プロジェクション行列。
		CMatrix		m_viewProjectionMatrix;				//!<ビュープロジェクション行列。
		CMatrix		m_viewMatrixInv;					//!<ビュー行列の逆行列。
		CMatrix		m_cameraRotation;					//!<カメラの回転行列。
		float			m_near;								//!<近平面。
		float			m_far;								//!<遠平面。
		float			m_viewAngle;						//!<画角(ラジアン)。
		float			m_aspect;							//!<アスペクト比。
		bool		m_isNeedUpdateProjectionMatrix;
	};
}
#endif //_TKCAMERA_H_