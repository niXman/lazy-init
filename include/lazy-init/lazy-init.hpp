
// Copyright (c) 2014, niXman (i dotty nixman doggy gmail dotty com)
// All rights reserved.
//
// This file is part of LAZY-INIT(https://github.com/niXman/lazy-init) project.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
//   Neither the name of the {organization} nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
T apply_impl(F &&f, Tuple&& tuple, seq<S...>) {
	return std::forward<F>(f)(std::get<S>(std::forward<Tuple>(tuple))...);
}

template<typename T, typename F, typename Tuple>
T apply(F &&f, Tuple &&tuple) {
	using indices = typename detail::gens<
		std::tuple_size<
			typename std::remove_reference<Tuple>::type
		>::value
	>::type;
	return apply_impl<T>(std::forward<F>(f), std::forward<Tuple>(tuple), indices());
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
			obj = detail::apply<T*>(
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
