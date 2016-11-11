/*!
 *@brief	�X�L�����f���f�[�^
 */
#ifndef _TKSKINMODELDATA_H_
#define _TKSKINMODELDATA_H_


namespace tkEngine{
	class CSkinModelMaterial;
	struct D3DXFRAME_DERIVED : public D3DXFRAME {
		D3DXMATRIX	CombinedTransformationMatrix;	//�����ςݍs��B
	};
	struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
		LPDIRECT3DTEXTURE9* ppTextures;
		LPD3DXMESH pOrigMesh;
		LPD3DXATTRIBUTERANGE pAttributeTable;
		DWORD NumAttributeGroups;
		DWORD NumInfl;
		LPD3DXBUFFER pBoneCombinationBuf;
		D3DXMATRIX** ppBoneMatrixPtrs;
		D3DXMATRIX* pBoneOffsetMatrices;
		DWORD NumPaletteEntries;
		DWORD iAttributeSW;
		CSkinModelMaterial* materials;
		CTexture* textures;
	};
	class CAnimation;
	/*!
	 *@brief	�X�L�����f���f�[�^
	 */
	class CSkinModelData : Noncopyable{
	public:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CSkinModelData();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CSkinModelData();
		/*!
		 * @brief	���f���f�[�^�����[�h�B
		 *@param[in]	filePath	�t�@�C���p�X�B
		 *@param[out]	anim		�A�j���[�V�����t�����f���f�[�^�̏ꍇ�A�A�j���[�V�����N���X���\�z����܂��B
		 */
		void LoadModelData( const char* filePath, CAnimation* anim );
		/*!
		 *@brief	�C���X�^���V���O�`����s�����߂̃f�[�^���쐬����B
		 *@param[in]	numInstance		�C���X�^���X�̐��B
		 *@param[in]	vertexElement	�C���X�^���V���O�`��p�̒��_�X�g���[���P�ɗ������ޒ��_�o�b�t�@�̒��_���C�A�E�g�B
		 */
		void CreateInstancingDrawData( int numInstance, SVertexElement* vertexElement );
		/*!
		* @brief	���f���f�[�^�̃N���[�����쐬�B
		* @details
		*	�X�P���g���͐V�����쐬����A���b�V���f�[�^�͋��ʂ̂��̂��g�p����܂��B
		*@param[in]		modelData		���f���f�[�^�B
		*/
		void CloneModelData( const CSkinModelData& modelData, CAnimation* anim);
		/*!
		* @brief	�����[�X�B
		*/
		void Release();
		LPD3DXFRAME GetFrameRoot()
		{
			return m_frameRoot;
		}
		//�e�X�g�B
		void SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
		/*!
		* @brief	�{�[���s����X�V�B
		*/
		void UpdateBoneMatrix( const CMatrix& matWorld );
		/*!
		* @brief	�C���X�^���V���O�`��p�̃f�[�^���X�V�B
		*@details
		* ���_�X�g���[���P�ɗ������ޒ��_�o�b�t�@�ɐݒ肷��f�[�^�ɂȂ�܂��B
		*@param[in]		data		���_�o�b�t�@�ɃR�s�[����f�[�^�B
		*/
		void UpdateInstancingDrawData(const void* data)
		{
			m_instanceVertexBuffer.Update(data);
		}
		/*!
		* @brief	�C���X�^���V���O�`��p�̎��Ɏg�p���钸�_��`���擾�B
		*/
		IDirect3DVertexDeclaration9* GetVertexDeclForInstancingDraw() const
		{
			return m_vertexDeclForInstancingDraw;
		}
		/*!
		* @brief	�C���X�^���V���O�`��p�̒��_�o�b�t�@���擾�B
		*@details
		* �X�g���[��1�ɗ������_�o�b�t�@�ł��B
		*/
		const CVertexBuffer& GetInstancingVertexBuffer() const
		{
			return (const_cast<CSkinModelData&>(*this)).GetInstancingVertexBuffer();
		}
		CVertexBuffer& GetInstancingVertexBuffer()
		{
			return m_instanceVertexBuffer;
		}
		/*!
		* @brief	�C���X�^���X�̐����擾�B
		*/
		int GetNumInstance() const
		{
			return m_numInstance;
		}
		/*!
		* @brief	���_�o�b�t�@�̃X�g���C�h���擾�B
		*/
		int GetVertexBufferStride() const
		{
			return m_vertexBufferStride;
		}
		/*!
		* @brief	�擪�̃��b�V�����擾�B
		*/
		LPD3DXMESH GetOrgMeshFirst() const;
		
		/*!
		* @brief	���̃��[���h�s�������
		*@details
		* ���O�Ō������s���Ă��邽�߁A�x���ł��B�p�ɂɌĂ΂Ȃ��悤�ɁB
		*@param[in]	boneName	���̖��O�B
		*@return ���������獜�̃��[���h�s��̃A�h���X��Ԃ��B������Ȃ������ꍇ��NULL��Ԃ��B
		*/
		CMatrix* FindBoneWorldMatrix(const char* boneName);
		/*!
		* @brief	�}�e���A���������B
		*@details
		* ���O�Ō������s���Ă��邽�߁A�x���ł��B�p�ɂɌĂ΂Ȃ��悤�ɁB
		*@param[in]	matName	�}�e���A�����B�f�B�t���[�Y�e�N�X�`�������}�e���A�����ɂȂ�܂��B
		*/
		CSkinModelMaterial* FindMaterial(const char* matName);
		/*!
		* @brief	���[�g�̃{�[�����擾�B
		*/
		CMatrix* GetRootBoneWorldMatrix()
		{
			TK_ASSERT(m_frameRoot != nullptr, "m_frameRoot is null");
			D3DXFRAME_DERIVED* frameDer = (D3DXFRAME_DERIVED*)m_frameRoot;
			return (CMatrix*)&frameDer->CombinedTransformationMatrix;
		}
		/*!
		* @brief	�X�L�����f���}�e���A����ǉ��B
		*@details
		* tkEngine�̒��ł����g�p����܂��B�O������g�p���Ȃ��悤�ɂ��Ă��������B
		*/
		void AddSkinModelMaterial(CSkinModelMaterial* mat)
		{
			m_materials.push_back(mat);
		}
		/*!
		* @brief	�X�L�����f���}�e���A���̃��X�g���擾�B
		*/
		const std::vector<CSkinModelMaterial*>& GetSkinModelMaterials() const
		{
			return m_materials;
		}
	private:
		CMatrix* FindBoneWorldMatrix(const char* boneName, LPD3DXFRAME frame);
		/*!
		* @brief	�I���W�i�����b�V�����擾�B
		*/
		LPD3DXMESH GetOrgMesh(LPD3DXFRAME frame) const;
		
		/*!
		* @brief	�I���W�i���̃X�P���g�����폜�B
		*@param[in]	frame	�X�P���g���B
		*/
		void DeleteSkeleton(LPD3DXFRAME frame);
		/*!
		* @brief	�X�P���g���̃N���[�����쐬�B
		*@param[out]	dstFrame		�N���[����̃X�P���g���̊i�[��B
		*@param[in]		srcFrame		�N���[�����̃X�P���g���B
		*/
		void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
		/*!
		* @brief	�X�P���g���̃N���[�����폜�B
		*@param[in]	frame		�폜����X�P���g���B
		*/
		void DeleteCloneSkeleton(LPD3DXFRAME frame);
		/*!
		 * @brief	�C���X�^���V���O�`��p�̃f�[�^���쐬�B
		 *@details
		 * frame�̌Z��A�q�����ċA�I�ɒ��ׂĂ����܂��B
		 *@param[in]	frame		�t���[��
		 *@param[in]	numInstance			�C���X�^���X�̐��B
		 *@param[in]	vertexElement	�C���X�^���V���O�`��p�̒��_�X�g���[���P�ɗ������ޒ��_�o�t�F�t�@�̒��_���C�A�E�g�B
		 *@return	true���A���Ă�����ċA�����I���B
		 */
		bool CreateInstancingDrawData(LPD3DXFRAME frame, int numInstance, SVertexElement* vertexElement );
		HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame);
	private:
		LPD3DXFRAME							m_frameRoot;		//�t���[�����[�g�B
		ID3DXAnimationController*			m_animController;	//�A�j���[�V�����R���g���[���B
		bool								m_isClone;			//�N���[���H
		CVertexBuffer			 			m_instanceVertexBuffer;			//�C���X�^���V���O�`��p�̒��_�o�b�t�@�̃��X�g�B
		IDirect3DVertexDeclaration9*		m_vertexDeclForInstancingDraw;	//�C���X�^���V���O�`����s�����̒��_��`�B
		int									m_numInstance;					//�C���X�^���X�̐��B
		int									m_vertexBufferStride;			//���_�o�b�t�@�̃X�g���C�h�B
		std::vector<CSkinModelMaterial*>	m_materials;					//�}�e���A���B
	};
}

#endif //_TKSKINMODELDATA_H_
