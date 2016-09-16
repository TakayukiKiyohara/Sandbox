/*!
 * @brief	エフェクトマネージャ
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkEffectManager.h"
#include "tkEngine/graphics/tkEffect.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/util/tkUtil.h"

namespace tkEngine{
	/*!
	 * @brief	コンストラクタ。
	 */
	CEffectManager::CEffectManager()
	{
	}
	/*!
	 * @brief	デストラクタ。
	 */
	CEffectManager::~CEffectManager()
	{
		Release();
	}
	/*!
	 *@brief	エフェクトのロード。
	 *@details
	 * 読み込み済みのエフェクトの場合はロードは行われず、</br>
	 * 既存のエフェクトが返ってきます。
	 *@param[in]	filePath	ロードするエフェクトのファイルパス。
	 *@return	ロードしたエフェクトファイル。
	 */
	CEffect* CEffectManager::LoadEffect( const char* filePath )
	{
		
		CEffect* pEffect = nullptr;
		int hash = CUtil::MakeHash(filePath);
		
		auto it = m_effectDictinary.find(hash);
		if (it == m_effectDictinary.end()) {
			//新規。
			LPD3DXEFFECT effect;
			LPD3DXBUFFER  compileErrorBuffer = nullptr;
			HRESULT hr = D3DXCreateEffectFromFile(
				CEngine::Instance().GetD3DDevice(),
				filePath,
				NULL,
				NULL,
#ifdef _DEBUG
				D3DXSHADER_DEBUG,
#else
				D3DXSHADER_SKIPVALIDATION,
#endif
				NULL,
				&effect,
				&compileErrorBuffer
				);
			if (FAILED(hr)) {
				MessageBox(nullptr, r_cast<char*>(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
				TK_ASSERT(SUCCEEDED(hr), "error");
			}
			
			pEffect = new CEffect(effect);
			std::pair<int, CEffect*> node(hash, pEffect);
			m_effectDictinary.insert(node);
		}
		else {
			//登録済み。
			pEffect = it->second;
		}
		return pEffect;
	}
	/*!
	 * @brief	リリース。
	 */
	void CEffectManager::Release()
	{
		for (auto p : m_effectDictinary) {
			delete p.second;
		}
		m_effectDictinary.clear();
	}
}