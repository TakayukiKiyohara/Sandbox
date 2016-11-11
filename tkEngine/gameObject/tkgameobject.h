/*!
 *@brief	ゲームオブジェクトの基底クラス。
 */

#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_


namespace tkEngine{
	class CGameObjectManager;
	class CRenderContext;
	typedef unsigned char	GameObjectPrio;
	/*!
	 *@brief	ゲームオブジェクト。
	 */
	class IGameObject : Noncopyable{
	public:
		IGameObject() :
			m_priority(0),
			m_isStart(false),
			m_isDead(false),
			m_isNewFromGameObjectManager(false)
		{
		}
		/*!
		 *@brief	デストラクタ
		 */
		virtual ~IGameObject()
		{
		}
	public:
		virtual void Start() {}
		/*!
		 *@brief	更新
		 */
		virtual void Update() = 0;
		/*!
		 *@brief	描画
		 */
		virtual void Render(CRenderContext& renderContext)
		{

		}
		/*!
		 *@brief	削除されるときに呼ばれる。
		 *@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
		 * デストラクタより前に実行されます。
		 */
		virtual void OnDestroy(){} 
		/*!
		*@brief	実行優先度を取得。
		*/
		GameObjectPrio GetPriority() const
		{
			return m_priority;
		}
	public:
		/*!
		 *@brief	インスタンスが生成された直後に呼ばれる関数。
		 *@details	コンストラクタが呼ばれた後で即呼ばれます。
		 */
		virtual void Awake(){}
		/*!
		 *@brief	Update関数が実行される前に呼ばれる更新関数。
		 */
		virtual void PreUpdate() {}
		/*!
		 *@brief	Update関数が実行された後で呼ばれる更新関数。
		 */
		virtual void PostUpdate() {} 
		/*!
		 *@brief	Render関数が実行される前に呼ばれる描画処理。
		 */
		virtual void PreRender( CRenderContext& renderContext ) {}
		/*!
		 *@brief	Render関数が実行された後で呼ばれる描画処理
		 *@details
		 * ポストエフェクトの後で実行されます。HUDなどポストエフェクトの影響を受けたくない描画物はここでレンダリングしてください。
		 */
		virtual void PostRender(CRenderContext& renderContext ) {}
		/*!
		*@brief	死亡フラグを立てる。
		*@details
		* この関数はエンジンの外からは実行しないでください。
		*/
		void SetDeadMark()
		{
			m_isDead = true;
		}
		/*!
		*@brief	死亡判定。
		*/
		bool IsDead() const
		{
			return m_isDead;
		}
	public:
		void PostRenderWrapper(CRenderContext& renderContext)
		{
			if (m_isStart && !m_isDead) {
				PostRender(renderContext);
			}
		}
		void RenderWrapper(CRenderContext& renderContext) 
		{
			if (m_isStart && !m_isDead) {
				Render(renderContext);
			}
		}
		void PreRenderWrapper(CRenderContext& renderContext)
		{
			if (m_isStart && !m_isDead) {
				PreRender(renderContext);
			}
		}
		void PostUpdateWrapper()
		{
			if (m_isStart && !m_isDead) {
				PostUpdate();
			}
		}
		void PreUpdateWrapper()
		{
			if (m_isStart && !m_isDead) {
				PreUpdate();
			}
		}
		void UpdateWrapper()
		{
			if (m_isStart && !m_isDead) {
				Update();
			}
		}
		void StartWrapper()
		{
			if (!m_isStart && !m_isDead) {
				Start();
				m_isStart = true;
			}
		}
		void SetMarkNewFromGameObjectManager()
		{
			m_isNewFromGameObjectManager = true;
		}
		bool IsNewFromGameObjectManager() const
		{
			return m_isNewFromGameObjectManager;
		}
		
		friend class CGameObjectManager;
	protected:
		GameObjectPrio	m_priority;			//!<実行優先度。
		bool m_isStart;						//!<Startの開始フラグ。
		bool m_isDead;						//!<死亡フラグ。
		bool m_isNewFromGameObjectManager;	//!<GameObjectManagerでnewされた。
		bool m_isRegist = false;			//!<GameObjectManagerに登録されている？
		
	};
}
#endif // _CGAMEOBJECT_H_