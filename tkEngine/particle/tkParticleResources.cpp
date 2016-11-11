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
			//新規
			LPDIRECT3DTEXTURE9 texDx;
			HRESULT hr = D3DXCreateTextureFromFileA(Engine().GetD3DDevice(), filePath, &texDx);
			if (FAILED(hr)) {
				//エラー用のテクスチャを表示。
				hr = D3DXCreateTextureFromFileA(Engine().GetD3DDevice(), "Assets/effect/error.png", &texDx);
				if (FAILED(hr)) {
					return nullptr;
				}
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