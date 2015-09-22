#ifndef CAMP_DETAIL_SINGLETON_HPP
#define CAMP_DETAIL_SINGLETON_HPP

//--------------------------------------------------------------------------------
#include <boost/noncopyable.hpp>

namespace camp {

	//--------------------------------------------------------------------------------
	
	template <typename T> struct Singleton : public boost::noncopyable {		
		Singleton() {
			uintptr_t offs = (uintptr_t)(T*)1 - (uintptr_t)(Singleton<T>*)(T*)1;
			_instance = (T*)((uintptr_t)this + offs);			
		}

		virtual ~Singleton() {
			_instance = nullptr;
		}
					
		static inline T&		inst() {
			return *_instance;
		}
		
		static inline T*		instptr() {
			return _instance;
		}
		
		static inline bool		valid() {
			return _instance != nullptr;
		}
	
	private:
		static T*		_instance;	
	};
	
	template <typename T> T*	Singleton<T>::_instance = nullptr;
	
	//--------------------------------------------------------------------------------

} // camp

//--------------------------------------------------------------------------------

#endif /*CAMP_DETAIL_SINGLETON_HPP*/
