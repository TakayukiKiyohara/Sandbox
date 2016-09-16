/*!
 * @brief	���_�o�b�t�@
 */

#ifndef _TKRENDERBUFFER_H_
#define _TKRENDERBUFFER_H_

namespace tkEngine{
	enum EVertexFormat {
		eVertexFormat_xyz		= D3DFVF_XYZ,
		eVertexFormat_wyzrhw	= D3DFVF_XYZRHW,
		eVertexFormat_xyzw		= D3DFVF_XYZW,
		eVertexFormat_normal	= D3DFVF_NORMAL,
		eVertexFormat_diffuse	= D3DFVF_DIFFUSE,
		eVertexFormat_tex0		= D3DFVF_TEX0,
		eVertexFormat_tex1		= D3DFVF_TEX1,
		eVertexFormat_tex2		= D3DFVF_TEX2,
		eVertexFormat_tex3		= D3DFVF_TEX3,
		eVertexFormat_tex4		= D3DFVF_TEX4,
		eVertexFormat_tex5		= D3DFVF_TEX5,
		eVertexFormat_tex6		= D3DFVF_TEX6,
		eVertexFormat_tex7		= D3DFVF_TEX7,
		eVertexFormat_tex8		= D3DFVF_TEX8,
		eVertexFormat_invalid	= 0xffffffff,

	};
	/*!
	 * @brief	���_�o�b�t�@
	 */
	class CVertexBuffer{
	public:
		CVertexBuffer();
		~CVertexBuffer();
		/*!
		 *@brief	���_�o�b�t�@�̍쐬�B
		 *@param[in]	numVertex		���_���B
		 *@param[in]	stride			���_�X�g���C�h�B
		 *@param[in]	vertexLayout	���_���C�A�E�g�B
		 *@param[in]	srcVertexBuffer	�\�[�X���_�o�b�t�@�B�쐬���ꂽ���_�o�b�t�@�ɃR�s�[����܂��BNULL���w��\�B
		 */
		void Create( 
			int numVertex, 
			int stride, 
			const SVertexElement* vertexLayout, 
			const void* pSrcVertexBuffer 
		);
		/*!
		 * @brief	���_�o�b�t�@�̊J���B
		 */
		void Release();
		/*!
		* @brief	LPDIRECT3DVERTEXBUFFER9���擾�B
		*/
		LPDIRECT3DVERTEXBUFFER9 GetBody()
		{
			return m_pVB;
		}
		/*!
		* @brief	���_�X�g���C�h���擾�B
		*/
		int GetStride() const
		{
			return m_stride;
		}
		/*!
		 * @brief	���_��`���擾�B
		 */
		SVertexDecralation* GetVertexDecl() 
		{
			return m_pVertexDecl;
		}
		/*!
		* @brief	���_�o�b�t�@���X�V�B
		*/
		void Update(const void* data);
	private:
		LPDIRECT3DVERTEXBUFFER9			m_pVB;				//!<���_�o�b�t�@�B
		SVertexDecralation* 			m_pVertexDecl;		//!<���_��`�B
		int 							m_stride;			//!<���_�X�g���C�h�B
		int 							m_numVertex;		//!<���_���B
		int 							m_size;				//!<�o�b�t�@�T�C�Y�B
	};
}
#endif // #define _TKRENDERBUFFER_H_