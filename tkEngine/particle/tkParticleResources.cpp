#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/particle/tkParticleResources.h"
#include "tkEngine/util/tkUtil.h"

namespace tkEngine{
	CParticleResources::CParticleResources()
	{
	}
	CParticleResources::~CParticleResources()
	{
		Release();
	}
	void CParticleResources::Release()
	{
		for (auto p : textureMap) {
			delete p.second;
		}
		textureMap.clear();
	}
	CTexture* CParticleResources::LoadTexture( const char* filePath )
	{
		int hash = CUtil::MakeHash(filePath);
		auto it = textureMap.find(hash);
		CTexture* tex = nullptr;
		if (it == textureMap.end()) {
			//V‹K
			LPDIRECT3DTEXTURE9 texDx;
			HRESULT hr = D3DXCreateTextureFromFileA(Engine().GetD3DDevice(), filePath, &texDx);
			if (FAILED(hr)) {
				MessageBox(nullptr, "failed texture", "error", MB_OK);
				TK_ASSERT(SUCCEEDED(hr), "error");
			}
			tex = new CTexture;
			tex->SetTextureDX(texDx);
			textureMap.insert(std::pair<int, CTexture*>(hash, tex));
		}
		else {
			tex = it->second;
		}
		return tex;
	}
}