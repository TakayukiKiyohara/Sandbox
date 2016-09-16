/*!
 *@brief	スケルトン
 */
#ifndef _TKSKELETON_H_
#define _TKSKELETON_H_


namespace tkEngine{
	/*!
	 *@brief	スケルトンクラス。
	 */
	class CSkeleton : Noncopyable{
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CSkeleton();
		/*!
		 *@brief	デストラクタ。
		 */
		~CSkeleton();
	private:
		LPD3DXFRAME m_frameRoot;		//フレームルート
	};
}

#endif //#ifndef _TKSKELETON_H_


