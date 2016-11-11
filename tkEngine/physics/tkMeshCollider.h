#pragma once
#include "tkEngine/Physics/tkICollider.h"

namespace tkEngine{

	/*!
	 * @brief	���b�V���R���C�_�[�B
	 */
	class CMeshCollider : public ICollider
	{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CMeshCollider();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CMeshCollider();
		/*!
		 * @brief	CSkinModel���烁�b�V���R���C�_�[�𐶐��B
		 *@param[in]	model		�X�L�����f���B
		 */
		void CreateFromSkinModel( CSkinModel* model, const CMatrix* offsetMatrix );
		btCollisionShape* GetBody() override
		{
			return meshShape;
		}
	private:
		typedef std::vector<CVector3>		VertexBuffer;		//���_�o�b�t�@�B
		typedef std::vector<unsigned int>	IndexBuffer;		//�C���f�b�N�X�o�b�t�@�B
		std::vector<VertexBuffer*>		vertexBufferArray;		//���_�o�b�t�@�̔z��B
		std::vector<IndexBuffer*>		indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
		btBvhTriangleMeshShape*			meshShape;				//���b�V���`��B
		btTriangleIndexVertexArray*		stridingMeshInterface;
	};
}
