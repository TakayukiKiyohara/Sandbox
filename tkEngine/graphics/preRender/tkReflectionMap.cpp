/*!
 *@brief	リフレクションマップ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkReflectionMap.h"
#include "tkEngine/graphics/tkCamera.h"

namespace tkEngine{
	/*!
	 *@brief	コンストラクタ。
	 */
	CReflectionMap::CReflectionMap() :
		m_isEnable(false),
		m_camera(nullptr)
	{
	}
	/*!
	 *@brief	デストラクタ。
	 */
	CReflectionMap::~CReflectionMap()
	{
	}
	/*!
	*@brief	開放。
	*/
	void CReflectionMap::Release()
	{
		m_renderTarget.Release();
	}
	/*!
	 *@brief	初期化。
	 */
	void CReflectionMap::Init(int w, int h)
	{
		m_renderTarget.Create(w, h, 1, FMT_A8R8G8B8, FMT_D16, MULTISAMPLE_NONE, 0);
		m_isEnable = true;
	}
	/*!
	 *@brief	描画。
	 */
	void CReflectionMap::Render(CRenderContext& renderContext)
	{
		if (m_camera) {
			CRenderTarget* rtBackup = renderContext.GetRenderTarget(0);
			renderContext.SetRenderTarget(0, &m_renderTarget);
			CMatrix projMatrix = m_camera->GetProjectionMatrix();
			CMatrix cameraMatrix = m_camera->GetViewMatrix();
			projMatrix.m[1][1] *= -1.0f;	//射影行列を-1倍して画像を反転させる。
			for (CSkinModel* model : m_models) {
				model->Draw(renderContext, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
			}
			m_models.clear();
			renderContext.SetRenderTarget(0, rtBackup);
		}
	}
}