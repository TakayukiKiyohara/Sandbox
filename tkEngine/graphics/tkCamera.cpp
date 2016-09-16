/*!
 * @brief	カメラ
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkCamera.h"
#include "tkEngine/tkEngine.h"

namespace tkEngine{
	CCamera::CCamera() :
		m_near(0.1f),
		m_far(1000.0f),
		m_viewAngle(CMath::DegToRad(60.0f)),
		m_aspect(0.0f),
		m_isNeedUpdateProjectionMatrix(true),
		m_position( CVector3::Zero),
		m_up( CVector3::Up),
		m_target( CVector3::Zero),
		m_viewMatrix( CMatrix::Identity ),
		m_projectionMatrix( CMatrix::Identity ),
		m_viewProjectionMatrix( CMatrix::Identity ),
		m_viewMatrixInv( CMatrix::Identity )
	{
	}
	CCamera::~CCamera()
	{
	}
	/*!
	 * @brief
	 */
	void CCamera::Update()
	{
		if(m_isNeedUpdateProjectionMatrix){
			float aspect = (float)CEngine::Instance().GetFrameBufferWidth() / (float)CEngine::Instance().GetFrameBufferHeight();
			//プロジェクション行列を計算。
			m_projectionMatrix.MakeProjectionMatrix(
				m_viewAngle,
				aspect,
				m_near,
				m_far
			);
		}
		//ビュー行列の算出
		m_viewMatrix.MakeLookAt( m_position, m_target, m_up );
		//ビュープロジェクション行列の作成。
		m_viewProjectionMatrix.Mul(m_viewMatrix, m_projectionMatrix);
		//ビュー行列の逆行列を計算。
		m_viewMatrixInv.Inverse( m_viewMatrix );
		//カメラの回転行列を取得。
		m_cameraRotation = m_viewMatrixInv;
		m_cameraRotation.m[3][0] = 0.0f;
		m_cameraRotation.m[3][1] = 0.0f;
		m_cameraRotation.m[3][2] = 0.0f;
		m_cameraRotation.m[3][3] = 1.0f;
	}
}

