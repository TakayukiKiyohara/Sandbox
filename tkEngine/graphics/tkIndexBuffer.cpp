/*!
 *@brief	インデックスバッファ
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkIndexBuffer.h"
#include "tkEngine/tkEngine.h"

namespace tkEngine {
	CIndexBuffer::CIndexBuffer() :
		m_pIB(nullptr)
	{

	}
	CIndexBuffer::~CIndexBuffer()
	{
		Release();
	}
	void CIndexBuffer::Create(int numIndex, EIndexFormat format, const void* pSrcIndexBuffer)
	{
		Release();
		int size = 0;
		D3DFORMAT d3dFormat;
		if (format == eIndexFormat16) {
			d3dFormat = D3DFMT_INDEX16;
			size = numIndex * 2;
		}
		else if (format == eIndexFormat32) {
			d3dFormat = D3DFMT_INDEX32;
			size = numIndex * 4;
		}
		LPDIRECT3DDEVICE9 d3dDevice = CEngine::Instance().GetD3DDevice();
		HRESULT hr = d3dDevice->CreateIndexBuffer(
			size,
			0,
			(D3DFORMAT)d3dFormat,
			D3DPOOL_DEFAULT,
			&m_pIB,
			nullptr
			);
		TK_ASSERT(SUCCEEDED(hr), "Failed CreateIndexBuffer!!");
		if (pSrcIndexBuffer != nullptr) {
			//ソースが指定されている。
			//インデックスバッファをロックしてコピー。
			void* pDstIndexBuffer;
			hr = m_pIB->Lock(0, 0, &pDstIndexBuffer, D3DLOCK_DISCARD);
			TK_ASSERT(SUCCEEDED(hr), "Failed IndexBuffer Lock!!");
			//まるっとコピー。
			memcpy(pDstIndexBuffer, pSrcIndexBuffer, size);
			m_pIB->Unlock();
		}
	}
	void CIndexBuffer::Release()
	{
		if (m_pIB != nullptr) {
			m_pIB->Release();
			m_pIB = nullptr;
		}
	}
}