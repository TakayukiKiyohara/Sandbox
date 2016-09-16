/*!
 * @brief	ユーティリティ関数。
 */
#ifndef _TKUTIL_H_
#define _TKUTIL_H_

namespace tkEngine{
	class CUtil{
	public:
		/*!
		 * @brief	文字列から32bitのハッシュ値を作成。
		 */
		static int MakeHash( const char* string )
		{
			int hash = 0;
			int l = (int)strlen(string);
			for( int i = 0; i < l; i++ ){
				hash = hash * 37 + string[i];
			}
			return hash;
		}
	};
}

#endif // _TKUTIL_H_