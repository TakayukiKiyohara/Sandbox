/*!
 * @brief	�v���~�e�B�u�B
 */

#ifndef _TKPRIMITIVE_H_
#define _TKPRIMITIVE_H_

#include "tkEngine/graphics/tkIndexBuffer.h"
#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine{
	/*!
	 * @brief	�v���~�e�B�u�B
	 */
	class CPrimitive{
	public:
		/*!
		 * @brief	�v���~�e�B�u�^�C�v�B
		 */
		enum EType{
			eTriangleList,		//!<�g���C�A���O�����X�g�B
			eTriangleStrip,		//!<�g���C�A���O���X�g���b�v�B
			eTypeNum,
		};
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CPrimitive();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CPrimitive();
		/*!
		 *@brief	�v���~�e�B�u�̍쐬�B
		 *@param[in]	primitiveType		�v���~�e�B�u�^�C�v�B
		 *@param[in]	numVertex			���_��
		 *@param[in]	vertexStride		���_�X�g���C�h
		 *@param[in]	vertexLayout		���_���C�A�E�g�BEVertexFormat�̘_���a��n���B
		 *@param[in]	pSrcVertexBuffer	�\�[�X���_�o�b�t�@�B
		 *@param[in]	numIndex			�C���f�b�N�X�̐�
		 *@param[in]	indexFormat			�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B
		 *@param[in]	pSrcIndexBuffer		�\�[�X�C���f�b�N�X�o�b�t�@�B
		 */
		void Create( 
			EType 					primitiveType,
			int 					numVertex,
			int 					vertexStride,
			const SVertexElement*	vertexLayout,
			void*					pSrcVertexBuffer,
			int 					numIndex,
			EIndexFormat			indexFormat,
			void*					pSrcIndexbuffer
		);
		/*!
		 * @brief	�����[�X�B
		 */
		void Release();
		/*!
		* @brief	���_�o�b�t�@�̎擾�B
		*/
		CVertexBuffer* GetVertexBuffer() 
		{
			return &m_vertexBuffer;
		}
		/*!
		* @brief	�C���f�b�N�X�o�b�t�@�̎擾�B
		*/
		CIndexBuffer*  GetIndexBuffer()
		{
			return &m_indexBuffer;
		}
		/*!
		* @brief	D3DPRIMITIVETYPE���擾�B
		*/
		D3DPRIMITIVETYPE GetD3DPrimitiveType() const
		{
			return m_d3dPrimitiveType;
		}
		/*!
		* @brief	���_�����擾�B
		*/
		int GetNumVertex() const
		{
			return m_numVertex;
		}
		/*!
		* @brief	�|���S�������擾�B
		*/
		int GetNumPolygon() const
		{
			return m_numPolygon;
		}
		/*!
		* @brief	���_��`���擾�B
		*/
		SVertexDecralation* GetVertexDecl()
		{
			return m_vertexBuffer.GetVertexDecl();
		}
	private:
		int					m_numVertex;		//!<���_���B
		int					m_vertexStride;		//!<���_�X�g���C�h�B
		int					m_numIndex;			//!<�C���f�b�N�X���B
		int					m_numPolygon;		//!<�|���S���̐��B
		CVertexBuffer		m_vertexBuffer;		//!<���_�o�b�t�@�B
		CIndexBuffer		m_indexBuffer;		//!<�C���f�b�N�X�o�b�t�@�B
		EType				m_type;				//!<�v���~�e�B�u�^�C�v�B
		D3DPRIMITIVETYPE	m_d3dPrimitiveType;	//!<D3DPRIMITIVETYPE�B
	};
}
#endif // _TKPRIMITIVE_H_