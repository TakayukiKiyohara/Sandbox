#ifndef _TK_CRENDERCONTEXT_H_
#define _TK_CRENDERCONTEXT_H_

#include "tkEngine/graphics/tkRenderCommandBuffer.h"

namespace tkEngine{
	class CVertexBuffer;
	class CIndexBuffer;
	class CPrimitive;
	class CRenderTarget;
	class CSkinModel;
	/*!
	* @brief	�����_�����O�R���e�L�X�g�B
	*/
	class CRenderContext : Noncopyable{
	public:
		CRenderContext() {
			memset(m_pCurrentRT, 0, sizeof(m_pCurrentRT));
		}
		~CRenderContext() {}
		/*!
		 *@brief	�C���X�^���X�̎擾�B
		 */
		static CRenderContext* GetInstance()
		{
			static CRenderContext instance;
			return &instance;
		}
		/*!
		 * @brief	���_��`��ݒ�BIDirect3DDevice9::SetVertexDeclaration�Ɠ����B
		 *@param[in]	vertexDecl		���_��`�B
		 */
		__inline void SetVertexDeclaration(SVertexDecralation* vertexDecl);
		/*!
		 * @brief	���_�X�g���[����ݒ�BIDirect3DDevice9::SetStreamSource�Ɠ����B
		 *@param[in]	streamNo		�X�g���[���ԍ��B
		 *@param[in]	pVB				���_�o�b�t�@�B
		 */
		__inline void SetStreamSource(int streamNo, CVertexBuffer* pVB);
		/*!
		 * @brief	�C���f�b�N�X�o�b�t�@��ݒ�BIDirect3DDevice9::SetIndices�Ɠ����B
		 *@param[in]	pIB		�C���f�b�N�X�o�b�t�@�B
		 */
		__inline void SetIndices(CIndexBuffer* pIB);
		/*!
		 * @brief	�C���f�b�N�X�o�b�t�@���Q�Ƃ��ăv���~�e�B�u��`��BIDirect3DDevice9::DrawIndexPrimitive�Ɠ����B
		 */
		__inline void DrawIndexedPrimitive(CPrimitive* prim);
		/*!
		* @brief	�����_�����O�^�[�Q�b�g�Ɛ[�x�X�e���V���o�b�t�@�̃N���A�BIDirect3DDevice9::Clear�Ɠ����B
		*@param[in]	Count		�N���A����̈�̃J�E���g�BpRects��NULL�ɐݒ肷��ꍇ�͂��̃p�����[�^���O�ɂ���B
		*@param[in] pRects		�����_�����O�^�[�Q�b�g���N���A����̈��������`�B</br>
		*						���̈�����NULL���w�肵���ꍇ�̓����_�����O�^�[�Q�b�g�S�̂��N���A�����B
		*@param[in]	Flags		�N���A����T�[�t�F�C�X�������t���O�B
		*@param[in]	Color		�����_�����O�^�[�Q�b�g�̃N���A�J���[�B32bit ARGB�B
		*@param[in]	Z			Z�o�b�t�@�̃N���A�l�B
		*@param[in]	Stencil		�X�e���V���o�b�t�@�̃N���A�l�B
		*/
		__inline void Clear(
			int Count,
			const D3DRECT  *pRects,
			int Flags,
			int Color,
			float Z,
			int Stencil);
		/*!
		* @brief	�r���[�|�[�g��ݒ�B�BIDirect3DDevice9::SetViewport�Ɠ����B
		*/
		__inline void SetViewport(const tkEngine::SViewport& viewport);
		/*!
		* @brief	�����_�[�X�e�[�g��ݒ�B
		*@param[in]	renderStateType		�ݒ肷�郌���_�����O�X�e�[�g�̎�ށB
		*@param[in]	value				�ݒ肷��l�B
		*/
		__inline void SetRenderState(ERenderStateType renderStateType, int value);
		/*!
		* @brief	�����_�����O�^�[�Q�b�g��ݒ�B
		*@param[in]	renderTargetIndex	�����_�����O�^�[�Q�b�g�̃C���f�b�N�X�B
		*@param[in]	pRT					�����_�����O�^�[�Q�b�g�B
		*/
		__inline void SetRenderTarget(int renderTargetIndex, CRenderTarget* pRT);
		/*!
		 * @brief	���b�V����`��B
		 *@param[in]	mesh		���b�V���B
		 *@param[in]	attrId		
		 */
		__inline void MeshDrawSubset(LPD3DXMESH mesh, DWORD attrId);
		/*!
		* @brief	�X�L�����f����`��B
		*@param[in]	skinMode		�X�L�����f���B
		*/
		__inline void DrawSkinModel(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		* @brief	�X�L�����f�����V���h�E�}�b�v�ɕ`��B
		*@param[in]	skinMode		�X�L�����f���B
		*/
		__inline void DrawSkinModelToShadowMap(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		* @brief	�X�L�����f���̃C���X�^���V���O�`��B
		*@param[in]	skinMode		�X�L�����f���B
		*/
		__inline void InstancingDrawSkinModel(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		/*!
		 *@brief	�������B
		 *@param[in]	pD3DDevice			Direct3D�f�o�C�X
		 *@param[in]	commandBufferSize	�R�}���h�o�b�t�@�̃T�C�Y�B
		 */
		void Init(LPDIRECT3DDEVICE9 pD3DDevice, int commandBufferSize);
		/*!
		 *@brief	�����_�����O�R�}���h��ǉ�
		 */
		template<class T>
		void SetRenderCommand( T& command )
		{
			static_assert( sizeof(T) <= 64, "command size is over 32byte" );
			m_commandBuffer.WriteCommand64( command );
		}
		/*!
		 *@brief	�R�}���h�o�b�t�@���T�u�~�b�g
		 */
		void SubmitCommandBuffer()
		{
			m_commandBuffer.Submit(m_pD3DDevice);
		}
		/*!
		*@brief	�R�}���h�o�b�t�@����A���P�[�g
		*/
		void* AllocFromCommandBuffer(int sizeInByte)
		{
			return m_commandBuffer.Alloc(sizeInByte);
		}
		/*!
		 * @brief	���݂̃����_�����O�^�[�Q�b�g���擾�B
		 *@param[in]	rtIndex		�����_�����O�^�[�Q�b�g�̃C���f�b�N�X�B
		 */
		CRenderTarget* GetRenderTarget( int rtIndex ) const
		{
			TK_ASSERT( rtIndex < NUM_MRT, "rtIndex is invalid!!" );
			return m_pCurrentRT[rtIndex];
		}
	private:
		static const int NUM_MRT = 4;					//!<MRT�͂Ƃ肠�����S�ɐ����B
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		CRenderCommandBuffer 	m_commandBuffer;
		CRenderTarget*			m_pCurrentRT[NUM_MRT];	//!<���݂̃����_�����O�^�[�Q�b�g�B
	};
}

#include "tkEngine/graphics/tkRenderCommand.h"
namespace tkEngine {
	__inline void CRenderContext::SetVertexDeclaration(SVertexDecralation* vertexDecl)
	{
		SetRenderCommand(CRenderCommand_SetVertexDeclaration(vertexDecl));
	}
	__inline void CRenderContext::SetStreamSource(int streamNo, CVertexBuffer* pVB)
	{
		SetRenderCommand(CRenderCommand_SetStreamSource(
			0,
			pVB
			));
	}
	__inline void CRenderContext::SetIndices(CIndexBuffer* pIB)
	{
		SetRenderCommand(CRenderCommand_SetIndices(
			pIB
			));
	}
	__inline void CRenderContext::DrawIndexedPrimitive(CPrimitive* prim)
	{
		SetRenderCommand(CRenderCommand_DrawIndexedPrimitive(
			prim
			));
	}
	__inline void CRenderContext::Clear(
		int Count,
		const D3DRECT *pRects,
		int Flags,
		int Color,
		float Z,
		int Stencil
		)
	{
		SetRenderCommand(CRenderCommand_Clear(
			Count, 
			pRects, 
			Flags,
			Color, 
			Z, 
			Stencil
		));
	}
	__inline void CRenderContext::SetViewport(const tkEngine::SViewport& viewport)
	{
		SetRenderCommand(CRenderCommand_SetViewport(viewport));
	}
	__inline void CRenderContext::SetRenderState(ERenderStateType renderStateType, int value)
	{
		SetRenderCommand(CRenderCommand_SetRenderState(renderStateType, value));
	}
	__inline void CRenderContext::SetRenderTarget(int renderTargetIndex, CRenderTarget* pRT)
	{
		TK_ASSERT(renderTargetIndex < NUM_MRT, "renderTargetIndex is invalid");
		m_pCurrentRT[renderTargetIndex] = pRT;
		SetRenderCommand(CRenderCommand_SetRenderTarget(renderTargetIndex, pRT));
		SetRenderCommand(CRenderCommand_SetDepthStencilSurface(renderTargetIndex, pRT));
	}
	/*!
	* @brief	���b�V����`��B
	*@param[in]	mesh		���b�V���B
	*/
	__inline void CRenderContext::MeshDrawSubset(LPD3DXMESH mesh, DWORD attrId)
	{
		SetRenderCommand(CRenderCommand_MeshDrawSubset(mesh, attrId));
	}
	/*!
	* @brief	�X�L�����f����`��B
	*@param[in]	skinMode		�X�L�����f���B
	*/
	__inline void CRenderContext::DrawSkinModel(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		SetRenderCommand(CRenderCommand_DrawSkinModel(*this, skinModel, viewMatrix, projMatrix));
	}
	/*!
	* @brief	�X�L�����f�����V���h�E�}�b�v�ɕ`��B
	*@param[in]	skinMode		�X�L�����f���B
	*/
	__inline void CRenderContext::DrawSkinModelToShadowMap(CSkinModel* skinModel, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		SetRenderCommand(CRenderCommand_DrawSkinModelToShadowMap(*this, skinModel, viewMatrix, projMatrix));
	}
}


#endif // _TK_CRENDERCONTEXT_H_