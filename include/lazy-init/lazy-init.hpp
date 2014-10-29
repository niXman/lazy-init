
#ifndef _lazy_init__lazy_init_hpp
#define _lazy_init__lazy_init_hpp

#include <tuple>
#include <type_traits>

/***************************************************************************/

namespace lazy_init {
namespace detail {

template<int ...>
struct seq {};

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> {};

template<int ...S>
struct gens<0, S...> {
	typedef seq<S...> type;
};

template<typename T, typename F, typename Tuple, int ...S>
T* invoke_impl(F &&f, Tuple&& tuple, seq<S...>) {
	return std::forward<F>(f)(std::get<S>(std::forward<Tuple>(tuple))...);
}

template<typename T, typename F, typename Tuple>
T* invoke(F &&f, Tuple &&tuple) {
	using indices = typename detail::gens<
		std::tuple_size<
			typename std::remove_reference<Tuple>::type
		>::value
	>::type;
	return invoke_impl<T>(std::forward<F>(f), std::forward<Tuple>(tuple), indices());
}

} // ns detail

/**********************************************************************************/

template <typename T, typename... Args>
struct lazy_init {
	using params_pack = std::tuple<Args...>;

	lazy_init(Args... args)
		:params(args...)
		,obj(nullptr)
	{}
	lazy_init(lazy_init<T, Args...>&& r)
		:params(std::move(r.params))
		,obj(r.obj)
	{ r.obj = nullptr; }
	lazy_init<T, Args...>& operator= (lazy_init<T, Args...>&& r) {
		params = std::move(r.params);
		if ( obj ) delete obj;
		obj = r.obj;
		r.obj = nullptr;
		return *this;
	}

	virtual ~lazy_init() { delete obj; }

	const T* get() const { return _get(); }
	T*       get()       { return _get(); }

	const T* operator->() const { return _get(); }
	T*       operator->()       { return _get(); }
	const T& operator* () const { return*_get(); }
	T&       operator* ()       { return*_get(); }

private:
	T* _get() const {
		if ( !obj )
			obj = detail::invoke<T>(
				 [](Args... args){ return new T(args...); }
				,params
			);

		return obj;
	}

private:
	params_pack params;
	mutable T *obj;

private:
	lazy_init(const lazy_init<T, Args...> &);
	lazy_init& operator= (const lazy_init<T, Args...> &);
};

/***************************************************************************/

template <typename T, typename... Args>
lazy_init<T, Args...> make_lazy_init(Args... args) {
	return lazy_init<T, Args...>(args...);
}

/***************************************************************************/

} // ns lazy_init

#endif // _lazy_init__lazy_init_hpp
