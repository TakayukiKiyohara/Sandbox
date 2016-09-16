/*!
 * @brief	���_�o�b�t�@
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine{
	CVertexBuffer::CVertexBuffer() :
		m_pVB(nullptr),
		m_stride(0),
		m_numVertex(0),
		m_size(0)
	{
	}
	CVertexBuffer::~CVertexBuffer()
	{
		Release();
	}
	void CVertexBuffer::Create(
			int numVertex, 
			int stride, 
			const SVertexElement* vertexLayout, 
			const void* pSrcVertexBuffer )
	{
		Release();
		m_numVertex = numVertex;
		m_stride = stride;
		m_size = m_numVertex * m_stride;
		LPDIRECT3DDEVICE9 d3dDevice = CEngine::Instance().GetD3DDevice();
		HRESULT hr = d3dDevice->CreateVertexBuffer(
			numVertex * stride,
			0,
			0,
			D3DPOOL_DEFAULT,
			&m_pVB,
			NULL
		);
		TK_ASSERT(SUCCEEDED(hr), "Failed CeateVertexBuffer!!" );
		if(pSrcVertexBuffer != nullptr){
			//�\�[�X���w�肳��Ă���B
			//���_�o�b�t�@�����b�N���ăR�s�[�B
			Update(pSrcVertexBuffer);
		}
		//���_��`���쐬�B
		d3dDevice->CreateVertexDeclaration(vertexLayout, &m_pVertexDecl);
	}
	void CVertexBuffer::Release()
	{
		if(m_pVB != nullptr){
			m_pVB->Release();
			m_pVB = nullptr;
		}
	}
	void CVertexBuffer::Update(const void* data)
	{
		void* pDstVertexBuffer;
		HRESULT hr = m_pVB->Lock(0, 0, &pDstVertexBuffer, D3DLOCK_DISCARD);
		TK_ASSERT(SUCCEEDED(hr), "Failed VertexBuffer Lock!!");
		//�܂���ƃR�s�[�B
		memcpy(pDstVertexBuffer, data, m_size);
		m_pVB->Unlock();
	}
}
