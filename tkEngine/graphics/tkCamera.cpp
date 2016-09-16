/*!
 * @brief	�J����
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
			//�v���W�F�N�V�����s����v�Z�B
			m_projectionMatrix.MakeProjectionMatrix(
				m_viewAngle,
				aspect,
				m_near,
				m_far
			);
		}
		//�r���[�s��̎Z�o
		m_viewMatrix.MakeLookAt( m_position, m_target, m_up );
		//�r���[�v���W�F�N�V�����s��̍쐬�B
		m_viewProjectionMatrix.Mul(m_viewMatrix, m_projectionMatrix);
		//�r���[�s��̋t�s����v�Z�B
		m_viewMatrixInv.Inverse( m_viewMatrix );
		//�J�����̉�]�s����擾�B
		m_cameraRotation = m_viewMatrixInv;
		m_cameraRotation.m[3][0] = 0.0f;
		m_cameraRotation.m[3][1] = 0.0f;
		m_cameraRotation.m[3][2] = 0.0f;
		m_cameraRotation.m[3][3] = 1.0f;
	}
}

