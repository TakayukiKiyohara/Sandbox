/*!
 * @brief	NoncopyÉ|ÉäÉVÅ[
 */
 
#ifndef _TKNONCOPYABLE_H_
#define _TKNONCOPYABLE_H_

namespace tkEngine{
	struct Noncopyable{
		Noncopyable() = default;
	    Noncopyable(const Noncopyable&) = delete;
    	Noncopyable& operator=(const Noncopyable&) = delete;
	};
}

#endif // _TKNONCOPYABLE_H_