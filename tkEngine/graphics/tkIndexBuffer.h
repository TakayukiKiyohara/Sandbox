/*!
 *@brief	�C���f�b�N�X�o�b�t�@
 */

#ifndef _TKINDEXBUFFER_H_
#define _TKINDEXBUFFER_H_

namespace tkEngine{
	enum EIndexFormat {
		eIndexFormat16 = D3DFMT_INDEX16,	//16bit�C���f�b�N�X�B
		eIndexFormat32 = D3DFMT_INDEX32,	//32bit�C���f�b�N�X�B
	};
	class CIndexBuffer{
	public:
		CIndexBuffer();
		~CIndexBuffer();
		/*!
		 *@brief	�C���f�b�N�X�o�b�t�@�̍쐬�B
		 *@param[in]	numIndex		�C���f�b�N�X�̐�
		 *@param[in]	format			�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B
		 *@param[in]	pSrcIndexBuffer	�\�[�X�C���f�b�N�X�o�b�t�@�B�쐬���ꂽ�C���f�b�N�X�o�b�t�@�ɃR�s�[����܂��BNULL���w��\�B
		 */
		void Create( int numIndex, EIndexFormat format, const void* pSrcIndexBuffer );
		/*!
		 *@brief	�C���f�b�N�X�o�b�t�@�̉���B
		 */
		void Release();
		/*!
		*@brief	LPDIRECT3DINDEXBUFFER9�̎擾�B
		*/
		LPDIRECT3DINDEXBUFFER9 GetBody()
		{
			return m_pIB;
		}
	private:
		LPDIRECT3DINDEXBUFFER9	m_pIB;	//!<�C���f�b�N�X�o�b�t�@�B
	};
}

#endif //_TKINDEXBUFFER_H_