/*!
 *@brief	CGameObject�̃}�l�[�W���[�B
 */

#ifndef _CGAMEOBJECTMANAGER_H_
#define _CGAMEOBJECTMANAGER_H_

#include "tkEngine/gameObject/tkGameObject.h"
#include "tkEngine/util/tkUtil.h"

namespace tkEngine{
	struct SRenderContextMap;
	/*!
	 *@brief	CGameObject�̃}�l�[�W��
	 */
	class CGameObjectManager : Noncopyable{
	private:
		CGameObjectManager() :
			m_gameObjectPriorityMax(0)
		{
		}
		~CGameObjectManager()
		{
		}
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̖��O�L�[���쐬�B
		*/
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = CUtil::MakeHash("Undefined");	//���O�L�[�B
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = CUtil::MakeHash(objectName);
			}
			return hash;
		}
	public:
		/*!
		 *@brief	�C���X�^���X�̎擾�B
		 */
		static CGameObjectManager& Instance()
		{
			static CGameObjectManager instance;
			return instance;
		}
		/*!
		*@brief	���s�B
		*@param[in]	renderContext		�����_�����O�R���e�L�X�g�B
		*@param[in]	numRenderContext	�����_�����O�R���e�L�X�g�̐��B
		*@param[in]	renderContextMap	�����_�����O�R���e�L�X�g�̃}�b�v�B
		*@param[in]	preRender			�v�������_�[
		*@param[in]	postEffect			�|�X�g�G�t�F�N�g�B
		*/
		void Execute( 
			CRenderContext* renderContext, 
			int numRenderContext, 
			const SRenderContextMap* renderContextMap,
			CPreRender& preRender,
			CPostEffect& postEffect
		);
		/*!
		 *@brief	�������B
		 *@param[in]	gameObjectPrioMax	�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B(255�܂�)
		 */
		void Init( int gameObjectPrioMax );
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̒ǉ��B
		*@param[in]	prio			���s�D�揇�ʁB
		*@param[in] go				�Q�[���I�u�W�F�N�g
		*@param[in] objectName		�I�u�W�F�N�g��
		*/
		void AddGameObject(GameObjectPrio prio, IGameObject* go, const char* objectName = nullptr)
		{
			go->Awake();
			unsigned int hash = MakeGameObjectNameKey(objectName);
			m_gameObjectListArray.at(prio).push_back(go);
		}
		/*!
		 *@brief	�Q�[���I�u�W�F�N�g��new
		 *@details
		 * ���̊֐����g�p����new�����I�u�W�F�N�g�͕K��DeleteGameObject�����s���邱�Ƃ�delete����܂��B
		 *@param[in]	prio		���s�D�揇�ʁB
		 *@param[in]	objectName	�I�u�W�F�N�g���B
		 */
		template<class T>
		T* NewGameObject(GameObjectPrio prio, const char* objectName = nullptr)
		{
			TK_ASSERT( prio <= m_gameObjectPriorityMax, "�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔���傫�����܂��B");
			T* newObject = new T();
			newObject->Awake();
			newObject->SetMarkNewFromGameObjectManager();
			unsigned int hash = MakeGameObjectNameKey(objectName);
			m_gameObjectListArray.at(prio).push_back(newObject);
			return newObject;
		}
		/*!
		 *@brief	�Q�[���I�u�W�F�N�g�̍폜�B
		 */
		void DeleteGameObject( IGameObject* gameObject )
		{
			gameObject->SetDeadMark();
			gameObject->OnDestroy();
			m_deleteObjectArray.at(gameObject->GetPriority()).push_back(gameObject);
		}
	private:
		/*!
		 *@brief	�Q�[���I�u�W�F�N�g�̍폜�����s�B
		 */
		void ExecuteDeleteGameObjects();
	private:
		typedef std::list<IGameObject*>	GameObjectList;
		std::vector<GameObjectList>	m_gameObjectListArray;	//!<�Q�[���I�u�W�F�N�g�̗D��x�t�����X�g�B
		std::vector<GameObjectList>	m_deleteObjectArray;	//!<�폜����I�u�W�F�N�g�̃��X�g�B
		GameObjectPrio				m_gameObjectPriorityMax;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔�B
		static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B
	};

	static inline CGameObjectManager& GameObjectManager()
	{
		return CGameObjectManager::Instance();
	}
	/*!
	 *@brief	�Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
	 *@param[in]	priority	�v���C�I���e�B�B
	 *@param[in]	objectName	�I�u�W�F�N�g���B
	 */
	template<class T>
	static inline T* NewGO( int priority, const char* objectName = nullptr)
	{
		return GameObjectManager().NewGameObject<T>( priority, objectName );
	}
	/*!
	 *@brief	�Q�[���I�u�W�F�N�g�폜�̃w���p�[�֐��B
	 * NewGO���g�p���č쐬�����I�u�W�F�N�g�͕K��DeleteGO�����s����悤�ɁB
	 *@param[in]	go		�폜����Q�[���I�u�W�F�N�g�B
	 */
	static inline void DeleteGO(IGameObject* go )
	{
		GameObjectManager().DeleteGameObject(go);
	}
	/*!
	 *@brief	�Q�[���I�u�W�F�N�g�̒ǉ��̃w���p�[�֐��B
	 *@param[in]	go			�ǉ�����Q�[���I�u�W�F�N�g�B
	 *@param[in]	priority	�v���C�I���e�B�B
	 *@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�B
	 */
	static inline void AddGO(int priority, IGameObject* go, const char* objectName = nullptr)
	{
		GameObjectManager().AddGameObject(priority, go, objectName);
	}
}
#endif // _CGAMEOBJECTMANAGER_H_
 