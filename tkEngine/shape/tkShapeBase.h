/*!
 *@brief	�`��̊�{�N���X�B
 */
#ifndef _TKSHAPEBASE_H_
#define _TKSHAPEBASE_H_

#include "tkEngine/graphics/tkLight.h"

namespace tkEngine{
	class CLight;
	/*!
	 *@brief	�`��̊�{�N���X�B
	 */
	class CShapeBase{
	public:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CShapeBase();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		virtual ~CShapeBase();
		/*!
		*@brief	���[���h�s��̎擾�B
		*/
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		*@brief	��]�s����擾�B
		*/
		const CMatrix& GetRotationMatrix() const
		{
			return m_rotationMatrix;
		}
		/*!
		*@brief	���W��ݒ�B
		*/
		void SetPosition(const CVector3& position)
		{
			m_position = position;
		}
		/*!
		*@brief	��]��ݒ�B
		*/
		void SetRotation(const CQuaternion& rotation)
		{
			m_rotation = rotation;
		}
		/*!
		*@brief	�v���~�e�B�u���擾�B
		*/
		CPrimitive* GetPrimitive()
		{
			return m_pPrimitive;
		}
		/*!
		 * @brief	�����[�X�B
		 */
		void Release();
		/*!
		*@brief	�X�V
		*/
		void UpdateWorldMatrix();
		/*!
		*@brief	�`��B
		*@param[in]	renderContext	�����_�����O�R���e�L�X�g�B
		*/
		void Render(CRenderContext& renderContext);
		/*!
		*@brief	���C�e�B���O����̕`��
		* �@������Ő������s���Ă���K�v����B
		*@param[in]	renderContext			�����_�����O�R���e�L�X�g�B
		*@param[in] viewProjectionMatrix	�r���[�v���W�F�N�V�����s��B
		*@param[in]	light					���C�g�B
		*@param[in]	isIluminance			���Ȕ�������B
		*@param[in]	isReceiveShadow			�e�̃��V�[�o�H
		*/
		void RenderLight( 
			CRenderContext& renderContext, 
			const CMatrix& viewProjectionMatrix,
			const CLight& light,
			bool isIluminance,
			bool isReceiveShadow
		);
		/*!
		*@brief	���C�e�B���O����̕`��BWVP�s��w��ŁB���O��WVP�s����쐬���Ă���Ȃ�A
		* ��������g�p����ق��������B
		* �@������Ő������s���Ă���K�v����B
		*@param[in]	renderContext			�����_�����O�R���e�L�X�g�B
		*@param[in] mWVP					WVP�s��B
		*@param[in]	light					���C�g�B
		*@param[in]	isIluminance			���Ȕ�������B
		*@param[in]	isReceiveShadow			�e�̃��V�[�o�H
		*@param[in]	pmWorldMatrix			�`����g���܂킵�ĕ����\�����Ă�����̂ɉe�𗎂Ƃ��Ƃ��́A���̕ϐ��Ƀ��[���h�s����w�肵�Ă��������B
		*									�w�肵�Ȃ��ƁA���ʂ̃��[���h�s�񂪎g�p����邽�߁A�Ӑ}���Ȃ����ʂɂȂ�܂��B
		*/
		void RenderLightWVP(
			CRenderContext& renderContext,
			const CMatrix& mWVP,
			const CLight& light = m_defaultLight,
			bool isIluminance = false,
			bool isReceiveShadow = false,
			const CMatrix* pmWorldMatrix = nullptr
		);
	protected:
		/*!
		*@brief	�G�t�F�N�g�쐬�B
		*@param[in]		hasNormal	�@����ێ����Ă��邩�B
		*/
		void CreateEffect(bool hasNormal);
	protected:
		bool m_isCreatePrimitive;		//!<�����Ńv���~�e�B�u���쐬�������ǂ����̃t���O�B
		CPrimitive*	m_pPrimitive;		//!<�v���~�e�B�u�B
		CVector3	m_position;			//!<���W�B
		CQuaternion	m_rotation;			//!<��]�B
		CMatrix		m_worldMatrix;		//!<���[���h�s��B
		CMatrix		m_rotationMatrix;	//!<���[���h��]�s��B
		CEffect*	m_pEffect;			//!<�G�t�F�N�g�B
		static CLight	m_defaultLight;		//!<�f�t�H���g���C�g�B
		static bool m_isInitDefaultLight;		//!<�f�t�H���g���C�g�̏������t���O�B
	};
	struct SMergeInfo{
		int	delVertNo;
		int replaceVertNo;
		int decReplaceVertNo;
	};
	struct SPolygonInfo {
		CVector3	normal;		//!<�ʖ@��
		int			vertNos[3];		//!<�ʂ��\�����钸�_�B
	};
	/*!
	*@brief	���_�o�b�t�@�̖@�����쐬�B(���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@��std::vector���g�p������)
	*@param[out]	vertexBuffer	���_�o�b�t�@�B
	*@param[in]		indexBuffer		�C���f�b�N�X�o�b�t�@�B
	*@param[in]		primType		�v���~�e�B�u�^�C�v
	*/
	template<class TVertex, class TIndex>
	void CreateVertexNormal(
		std::vector<TVertex>& vertexBuffer, 
		const std::vector<TIndex>& indexBuffer,
		CPrimitive::EType primType )
	{	
		//�ʖ@�������߂�B
		TK_ASSERT( primType == CPrimitive::eTriangleList, "not implement TraiangleStrip yet...");
		int numFace = (int)indexBuffer.size() / 3;
		std::unique_ptr<SPolygonInfo[]> polygonInfo;
		std::vector<std::list<SPolygonInfo>>	polygoninfos;
		polygoninfos.resize(vertexBuffer.size());
		for (int i = 0; i < numFace; i++) {
			CVector3 pos[3];
			int t = i * 3;
			SPolygonInfo polyInfo;
			polyInfo.vertNos[0] = indexBuffer.at(t);
			polyInfo.vertNos[1] = indexBuffer.at(t+1);
			polyInfo.vertNos[2] = indexBuffer.at(t+2);
			if (polyInfo.vertNos[0] == polyInfo.vertNos[1]
				|| polyInfo.vertNos[0] == polyInfo.vertNos[2]
				|| polyInfo.vertNos[1] == polyInfo.vertNos[2]
				) {
				//�k�ރ|���S���B
				continue;
			}
			CVector3 normal(0.0f, 0.0f, 0.0f);
			CVector3 vertPos[3];
			for (int k = 0; k < 3; k++) {
				const TVertex& vtx = vertexBuffer.at(polyInfo.vertNos[k]);
				vertPos[k].Set(	vtx.pos[0], vtx.pos[1], vtx.pos[2] );
			}
			CVector3 t0, t1;
			t0.Subtract(vertPos[1], vertPos[0]);
			t1.Subtract(vertPos[2], vertPos[0]);
			t0.Normalize();
			t1.Normalize();
			polyInfo.normal.Cross(t0, t1);
			polygoninfos.at(polyInfo.vertNos[0]).push_back(polyInfo);
			polygoninfos.at(polyInfo.vertNos[1]).push_back(polyInfo);
			polygoninfos.at(polyInfo.vertNos[2]).push_back(polyInfo);
		}
		//���_�@�������߂�B
		for (int vertNo = 0; vertNo < (int)vertexBuffer.size(); vertNo++) {
			std::list<SPolygonInfo>& polygonInfo = polygoninfos.at(vertNo);
			CVector3 normal(0.0f, 0.0f, 0.0f);
			for (const auto& p : polygonInfo) {
				normal.Add(p.normal);
			}
			normal.Div(s_cast<float>(polygonInfo.size()));
			float* pNormal = vertexBuffer.at(vertNo).normal;
			pNormal[0] = normal.x;
			pNormal[1] = normal.y;
			pNormal[2] = normal.z;
			pNormal[3] = 1.0f;
		}

	}
	/*!
	*@brief	���_�o�b�t�@�̖@�����쐬�B(���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�ɔz����g�p������)
	*@param[out]	vertexBuffer	���_�o�b�t�@�B
	*@param[in]		numVertex		���_���B
	*@param[in]		indexBuffer		�C���f�b�N�X�o�b�t�@�B
	*@param[in]		numIndex		�C���f�b�N�X���B
	*@param[in]		primType		�v���~�e�B�u�^�C�v
	*/
	template<class TVertex, class TIndex>
	void CreateVertexNormalArray(
		TVertex* vertexBuffer,
		int numVertex,
		TIndex* indexBuffer,
		int numIndex,
		CPrimitive::EType primType)
	{
		//�ʖ@�������߂�B
		TK_ASSERT(primType == CPrimitive::eTriangleList, "not implement TraiangleStrip yet...");
		int numFace = numIndex / 3;
		std::unique_ptr<SPolygonInfo[]> polygonInfo;
		std::vector<std::list<SPolygonInfo>>	polygoninfos;
		polygoninfos.resize(numVertex);
		for (int i = 0; i < numFace; i++) {
			CVector3 pos[3];
			int t = i * 3;
			SPolygonInfo polyInfo;
			polyInfo.vertNos[0] = indexBuffer[t];
			polyInfo.vertNos[1] = indexBuffer[t + 1];
			polyInfo.vertNos[2] = indexBuffer[t + 2];
			if (polyInfo.vertNos[0] == polyInfo.vertNos[1]
				|| polyInfo.vertNos[0] == polyInfo.vertNos[2]
				|| polyInfo.vertNos[1] == polyInfo.vertNos[2]
				) {
				//�k�ރ|���S���B
				continue;
			}
			CVector3 normal(0.0f, 0.0f, 0.0f);
			CVector3 vertPos[3];
			for (int k = 0; k < 3; k++) {
				const TVertex& vtx = vertexBuffer[polyInfo.vertNos[k]];
				vertPos[k].Set(vtx.pos[0], vtx.pos[1], vtx.pos[2]);
			}
			CVector3 t0, t1;
			t0.Subtract(vertPos[1], vertPos[0]);
			t1.Subtract(vertPos[2], vertPos[0]);
			t0.Normalize();
			t1.Normalize();
			polyInfo.normal.Cross(t0, t1);
			polyInfo.normal.Normalize();
			polygoninfos.at(polyInfo.vertNos[0]).push_back(polyInfo);
			polygoninfos.at(polyInfo.vertNos[1]).push_back(polyInfo);
			polygoninfos.at(polyInfo.vertNos[2]).push_back(polyInfo);
		}
		//���_�@�������߂�B
		for (int vertNo = 0; vertNo < numVertex; vertNo++) {
			std::list<SPolygonInfo>& polygonInfo = polygoninfos.at(vertNo);
			CVector3 normal(0.0f, 0.0f, 0.0f);
			for (const auto& p : polygonInfo) {
				normal.Add(p.normal);
			}
			normal.Div(s_cast<float>(polygonInfo.size()));
			float* pNormal = vertexBuffer[vertNo].normal;
			pNormal[0] = normal.x;
			pNormal[1] = normal.y;
			pNormal[2] = normal.z;
			pNormal[3] = 1.0f;
		}

	}
	/*!
	 *@brief	���_�}�[�W�B
	 *@details
	 * �v�Z�ʂ͒��_����N�Ƃ����Ƃ���O(N^2)�ɂȂ�̂ŁA���_����������ƒx���Ȃ�B</br>
	 * ���x�����ɂȂ�����񕪖،����B
	 *@param[in, out]	vertexBuffer		�}�[�W�Ώۂ̒��_�o�b�t�@�B
	 *@param[in, out]	indexBuffer			�}�[�W�Ώۂ̃C���f�b�N�X�o�b�t�@�B
	 *@param[in]		margeLenThreshold	�}�[�W���钸�_���m�̋�����臒l�B���̒l�ȉ��̒��_�̓}�[�W����܂��B
	 */
	template<class TVertex, class TIndex>
	void MergeVertex( std::vector<TVertex>& vertexBuffer, std::vector<TIndex>& indexBuffer, float margeLenThreshold )
	{
	#if 1	//�������̓}�[�W���ꂽ���_�͍폜����B
		float margeLenThresholdSq = margeLenThreshold * margeLenThreshold;
		typedef std::pair<int, int>	SMergeInfo;			//�}�[�W���B�폜���钸�_�ԍ��ƁA�u�������钸�_�ԍ��̃y�A���B
		typename std::vector<bool> deleteVertexFlags;	//���_�폜�̃t���O�̃��X�g�B
		typename std::vector<int>  deleteCount;			//���g��菬�����ԍ��̒��_�������폜���ꂽ���̃J�E���g����
		std::list<SMergeInfo> mergeInfos;				//�폜���钸�_�ԍ��̃��X�g�B
		deleteVertexFlags.resize( vertexBuffer.size() );
		deleteCount.resize( vertexBuffer.size() );
		for (auto& p : deleteVertexFlags) {
			p = false;
		}
		for (auto& p : deleteCount) {
			p = 0;
		}
		int vertNo = 0;
		for( int vertNo = 0; vertNo != vertexBuffer.size(); vertNo++ ){
			if(deleteVertexFlags[vertNo]){
				//�폜�\��̒��_�Ȃ̂Œ��ׂ�K�v�Ȃ��B
				continue;
			}
			const TVertex& v0 = vertexBuffer[vertNo];
			CVector3 vPos0;
			CVector3 vLen;
			vPos0.Set( v0.pos[0], v0.pos[1], v0.pos[2] );
			for( int delVertNo = 0; delVertNo < (int)vertexBuffer.size(); delVertNo++ ){
				if(vertNo != delVertNo && !deleteVertexFlags[delVertNo]){
					//���_���m�̋����𒲂ׂ�B
					const TVertex& v1 = vertexBuffer[delVertNo];
					vLen.Set( v1.pos[0], v1.pos[1], v1.pos[2] );
					vLen.Subtract(vPos0, vLen);
					float lenSq = vLen.LengthSq();
					if(lenSq < margeLenThresholdSq ){
						deleteVertexFlags[delVertNo] = true;	//�폜����������B
						//�}�[�W�����쐬����B
						SMergeInfo mergeInfo;
						mergeInfo.first = delVertNo;
						mergeInfo.second = vertNo;
						mergeInfos.push_back(mergeInfo);
					}
				}
			}
		}
		for( auto& mergeInfoIt : mergeInfos ){
			for( TIndex index = mergeInfoIt.first+1; index < (int)deleteCount.size(); index++ ){
				deleteCount[index]++;
			}
		}
		std::vector<TVertex> vertexBufferTmp = vertexBuffer;
		vertexBuffer.clear();
		for( int i = 0; i < (int)vertexBufferTmp.size(); i++ ){
			if( !deleteVertexFlags[i] ){
				vertexBuffer.push_back(vertexBufferTmp[i]);
			}
		}
		//���_���폜�����̂ŃC���f�b�N�X�o�b�t�@�𒲐��B
		{
			for( auto& mergeInfoIt : mergeInfos ){
				//�C���f�b�N�X�o�b�t�@����폜�Ώۂ̒��_��T���A�폜�Ώۂ̒��_���ԍ����傫�����̂͌��Z����B�B
				for( auto& indexIt : indexBuffer ){
					if( mergeInfoIt.first == indexIt ){
						//�폜�Ώ�
						indexIt = mergeInfoIt.second;
					}
				}
			}
		}
		for( auto& indexIt : indexBuffer ){
			indexIt -= deleteCount[indexIt];
		}
	#else	//�������̓}�[�W�������_�͍폜���Ȃ��ŁA�C���f�b�N�X�o�b�t�@���g�p���ă}�[�W���邾��
		float margeLenThresholdSq = margeLenThreshold * margeLenThreshold;
		typedef std::pair<int, int>	SMergeInfo;			//�}�[�W���B�폜���钸�_�ԍ��ƁA�u�������钸�_�ԍ��̃y�A���B
		typename std::vector<bool> deleteVertexFlags;	//���_�폜�̃t���O�̃��X�g�B
		std::list<SMergeInfo> mergeInfos;				//�폜���钸�_�ԍ��̃��X�g�B
		deleteVertexFlags.resize(vertexBuffer.size());
		for (auto& p : deleteVertexFlags) {
			p = false;
		}
		int vertNo = 0;
		for( int vertNo = 0; vertNo != vertexBuffer.size(); vertNo++ ){
			if(deleteVertexFlags[vertNo]){
				//�폜�\��̒��_�Ȃ̂Œ��ׂ�K�v�Ȃ��B
				continue;
			}
			const TVertex& v0 = vertexBuffer[vertNo];
			CVector3 vPos0;
			CVector3 vLen;
			vPos0.Set( v0.pos[0], v0.pos[1], v0.pos[2] );
			for( int delVertNo = 0; delVertNo < vertexBuffer.size(); delVertNo++ ){
				if(vertNo != delVertNo && !deleteVertexFlags[delVertNo]){
					//���_���m�̋����𒲂ׂ�B
					const TVertex& v1 = vertexBuffer[delVertNo];
					vLen.Set( v1.pos[0], v1.pos[1], v1.pos[2] );
					vLen.Subtract(vPos0, vLen);
					float lenSq = vLen.LengthSq();
					if(lenSq < margeLenThresholdSq ){
						deleteVertexFlags[delVertNo] = true;	//�폜����������B
						//�}�[�W�����쐬����B
						SMergeInfo mergeInfo;
						mergeInfo.first = delVertNo;
						mergeInfo.second = vertNo;
						mergeInfos.push_back(mergeInfo);
					}
				}
			}
		}
		//���_���폜�����̂ŃC���f�b�N�X�o�b�t�@�𒲐��B
		{
			for( auto& mergeInfoIt : mergeInfos ){
				//�C���f�b�N�X�o�b�t�@����폜�Ώۂ̒��_��T���A�폜�Ώۂ̒��_���ԍ����傫�����̂͌��Z����B�B
				for( auto& indexIt : indexBuffer ){
					if( mergeInfoIt.first == indexIt ){
						//�폜�Ώ�
						indexIt = mergeInfoIt.second;
					}
				}
			}
		}
	#endif
	}
}
#endif //_TKSHAPEBASE_H_