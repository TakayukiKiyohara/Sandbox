/*!
 * @brief	エフェクトマネージャー
 */

#ifndef _TKEFFECTMANAGER_H_
#define _TKEFFECTMANAGER_H_

namespace tkEngine{
	class CEffect;
	/*!
	 * @brief	エフェクトマネージャ。
	 */
	class CEffectManager : Noncopyable{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CEffectManager();
		/*!
		 * @brief	デストラクタ。
		 */
		~CEffectManager();
		/*!
		 *@brief	エフェクトのロード。
		 *@details
		 * 読み込み済みのエフェクトの場合はロードは行われず、</br>
		 * 既存のエフェクトが返ってきます。
		 *@param[in]	filePath	ロードするエフェクトのファイルパス。
		 *@return	ロードしたエフェクトファイル。
		 */
		CEffect* LoadEffect( const char* filePath );
		/*!
		 * @brief	リリース。
		 */
		void Release();
	private:
		std::map<int, CEffect*>		m_effectDictinary;	//!<CEffectのインスタンスのディクショナリ。ファイルパスのハッシュ値をキーに持ち、CEffectのインスタンスを値に持つ。
	};
}

#endif /_TKEFFECTMANAGER_H_