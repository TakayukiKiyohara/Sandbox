/*!
 *@brief	スキンモデルデータ
 */
#ifndef _TKSKINMODELDATA_H_
#define _TKSKINMODELDATA_H_


namespace tkEngine{
	class CSkinModelMaterial;
	struct D3DXFRAME_DERIVED : public D3DXFRAME {
		D3DXMATRIX	CombinedTransformationMatrix;	//合成済み行列。
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
	 *@brief	スキンモデルデータ
	 */
	class CSkinModelData : Noncopyable{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CSkinModelData();
		/*!
		 *@brief	デストラクタ。
		 */
		~CSkinModelData();
		/*!
		 * @brief	モデルデータをロード。
		 *@param[in]	filePath	ファイルパス。
		 *@param[out]	anim		アニメーション付きモデルデータの場合、アニメーションクラスも構築されます。
		 */
		void LoadModelData( const char* filePath, CAnimation* anim );
		/*!
		 *@brief	インスタンシング描画を行うためのデータを作成する。
		 *@param[in]	numInstance		インスタンスの数。
		 *@param[in]	vertexElement	インスタンシング描画用の頂点ストリーム１に流し込む頂点バッファの頂点レイアウト。
		 */
		void CreateInstancingDrawData( int numInstance, SVertexElement* vertexElement );
		/*!
		* @brief	モデルデータのクローンを作成。
		* @details
		*	スケルトンは新しく作成され、メッシュデータは共通のものが使用されます。
		*@param[in]		modelData		モデルデータ。
		*/
		void CloneModelData( const CSkinModelData& modelData, CAnimation* anim);
		/*!
		* @brief	リリース。
		*/
		void Release();
		LPD3DXFRAME GetFrameRoot()
		{
			return m_frameRoot;
		}
		//テスト。
		void SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
		/*!
		* @brief	ボーン行列を更新。
		*/
		void UpdateBoneMatrix( const CMatrix& matWorld );
		/*!
		* @brief	インスタンシング描画用のデータを更新。
		*@details
		* 頂点ストリーム１に流し込む頂点バッファに設定するデータになります。
		*@param[in]		data		頂点バッファにコピーするデータ。
		*/
		void UpdateInstancingDrawData(const void* data)
		{
			m_instanceVertexBuffer.Update(data);
		}
		/*!
		* @brief	インスタンシング描画用の時に使用する頂点定義を取得。
		*/
		IDirect3DVertexDeclaration9* GetVertexDeclForInstancingDraw() const
		{
			return m_vertexDeclForInstancingDraw;
		}
		/*!
		* @brief	インスタンシング描画用の頂点バッファを取得。
		*@details
		* ストリーム1に流す頂点バッファです。
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
		* @brief	インスタンスの数を取得。
		*/
		int GetNumInstance() const
		{
			return m_numInstance;
		}
		/*!
		* @brief	頂点バッファのストライドを取得。
		*/
		int GetVertexBufferStride() const
		{
			return m_vertexBufferStride;
		}
		/*!
		* @brief	先頭のメッシュを取得。
		*/
		LPD3DXMESH GetOrgMeshFirst() const;
		
		/*!
		* @brief	骨のワールド行列を検索
		*@details
		* 名前で検索を行っているため、遅いです。頻繁に呼ばないように。
		*@param[in]	boneName	骨の名前。
		*@return 見つかったら骨のワールド行列のアドレスを返す。見つからなかった場合はNULLを返す。
		*/
		CMatrix* FindBoneWorldMatrix(const char* boneName);
		/*!
		* @brief	マテリアルを検索。
		*@details
		* 名前で検索を行っているため、遅いです。頻繁に呼ばないように。
		*@param[in]	matName	マテリアル名。ディフューズテクスチャ名がマテリアル名になります。
		*/
		CSkinModelMaterial* FindMaterial(const char* matName);
		/*!
		* @brief	ルートのボーンを取得。
		*/
		CMatrix* GetRootBoneWorldMatrix()
		{
			TK_ASSERT(m_frameRoot != nullptr, "m_frameRoot is null");
			D3DXFRAME_DERIVED* frameDer = (D3DXFRAME_DERIVED*)m_frameRoot;
			return (CMatrix*)&frameDer->CombinedTransformationMatrix;
		}
		/*!
		* @brief	スキンモデルマテリアルを追加。
		*@details
		* tkEngineの中でだけ使用されます。外部から使用しないようにしてください。
		*/
		void AddSkinModelMaterial(CSkinModelMaterial* mat)
		{
			m_materials.push_back(mat);
		}
		/*!
		* @brief	スキンモデルマテリアルのリストを取得。
		*/
		const std::vector<CSkinModelMaterial*>& GetSkinModelMaterials() const
		{
			return m_materials;
		}
	private:
		CMatrix* FindBoneWorldMatrix(const char* boneName, LPD3DXFRAME frame);
		/*!
		* @brief	オリジナルメッシュを取得。
		*/
		LPD3DXMESH GetOrgMesh(LPD3DXFRAME frame) const;
		
		/*!
		* @brief	オリジナルのスケルトンを削除。
		*@param[in]	frame	スケルトン。
		*/
		void DeleteSkeleton(LPD3DXFRAME frame);
		/*!
		* @brief	スケルトンのクローンを作成。
		*@param[out]	dstFrame		クローン先のスケルトンの格納先。
		*@param[in]		srcFrame		クローン元のスケルトン。
		*/
		void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
		/*!
		* @brief	スケルトンのクローンを削除。
		*@param[in]	frame		削除するスケルトン。
		*/
		void DeleteCloneSkeleton(LPD3DXFRAME frame);
		/*!
		 * @brief	インスタンシング描画用のデータを作成。
		 *@details
		 * frameの兄弟、子供を再帰的に調べていきます。
		 *@param[in]	frame		フレーム
		 *@param[in]	numInstance			インスタンスの数。
		 *@param[in]	vertexElement	インスタンシング描画用の頂点ストリーム１に流し込む頂点バフェファの頂点レイアウト。
		 *@return	trueが帰ってきたら再帰処理終了。
		 */
		bool CreateInstancingDrawData(LPD3DXFRAME frame, int numInstance, SVertexElement* vertexElement );
		HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame);
	private:
		LPD3DXFRAME							m_frameRoot;		//フレームルート。
		ID3DXAnimationController*			m_animController;	//アニメーションコントローラ。
		bool								m_isClone;			//クローン？
		CVertexBuffer			 			m_instanceVertexBuffer;			//インスタンシング描画用の頂点バッファのリスト。
		IDirect3DVertexDeclaration9*		m_vertexDeclForInstancingDraw;	//インスタンシング描画を行う時の頂点定義。
		int									m_numInstance;					//インスタンスの数。
		int									m_vertexBufferStride;			//頂点バッファのストライド。
		std::vector<CSkinModelMaterial*>	m_materials;					//マテリアル。
	};
}

#endif //_TKSKINMODELDATA_H_
