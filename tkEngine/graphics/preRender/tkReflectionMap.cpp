/*!
 *@brief	���t���N�V�����}�b�v�B
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkReflectionMap.h"
#include "tkEngine/graphics/tkCamera.h"

namespace tkEngine{
	/*!
	 *@brief	�R���X�g���N�^�B
	 */
	CReflectionMap::CReflectionMap() :
		m_isEnable(false),
		m_camera(nullptr)
	{
	}
	/*!
	 *@brief	�f�X�g���N�^�B
	 */
	CReflectionMap::~CReflectionMap()
	{
	}
	/*!
	*@brief	�J���B
	*/
	void CReflectionMap::Release()
	{
		m_renderTarget.Release();
	}
	/*!
	 *@brief	�������B
	 */
	void CReflectionMap::Init(int w, int h)
	{
		m_renderTarget.Create(w, h, 1, FMT_A8R8G8B8, FMT_D16, MULTISAMPLE_NONE, 0);
		m_isEnable = true;
	}
	/*!
	 *@brief	�`��B
	 */
	void CReflectionMap::Render(CRenderContext& renderContext)
	{
		if (m_camera) {
			CRenderTarget* rtBackup = renderContext.GetRenderTarget(0);
			renderContext.SetRenderTarget(0, &m_renderTarget);
			CMatrix projMatrix = m_camera->GetProjectionMatrix();
			CMatrix cameraMatrix = m_camera->GetViewMatrix();
			projMatrix.m[1][1] *= -1.0f;	//�ˉe�s���-1�{���ĉ摜�𔽓]������B
			for (CSkinModel* model : m_models) {
				model->Draw(renderContext, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
			}
			m_models.clear();
			renderContext.SetRenderTarget(0, rtBackup);
		}
	}
}