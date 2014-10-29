
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

#include <lazy-init/lazy-init.hpp>

#include <iostream>
#include <cassert>

/***************************************************************************/

struct type0 {
	type0()
		:v()
	{}

	int v;
};

struct type1 {
	type1(int &v)
		:v(v)
	{ v = v*2; }

	int &v;
};

struct type2 {
	type2(int v0, int &v1)
		:v0(v0)
		,v1(v1)
	{ v1 = v1*2; }

	int  v0;
	int &v1;
};

struct noncopyable {
	noncopyable() {}
private:
	noncopyable(const noncopyable &);
	noncopyable& operator= (const noncopyable &);
};

struct type3 {
	type3(noncopyable &v)
		:v(v)
	{}

	noncopyable &v;
};

/***************************************************************************/

int main() {
	{
		auto t = lazy_init::make_lazy_init<type0>();
		assert(t->v == 0);
	}
	{
		int v = 3;
		auto t = lazy_init::make_lazy_init<type1, int&>(v);
		assert(v == 3);
		assert(t->v == 3*2);
	}
	{
		int v0 = 2;
		int v1 = 3;
		auto t = lazy_init::make_lazy_init<type2, int, int&>(v0, v1);
		assert(v1 == 3);
		assert(t->v1 == 3*2);
	}
	{
//		noncopyable v;
//		auto t = lazy_init::make_lazy_init<type3>(v);
	}
	{
		auto t = lazy_init::make_lazy_init<int>(3);
		assert(*t == 3);
	}
	{
		int v0 = 3;
		int v1 = 4;
		auto t0 = lazy_init::make_lazy_init<type1, int&>(v0);
		assert(v0 == 3);
		auto t1 = lazy_init::make_lazy_init<type1, int&>(v1);
		assert(v1 == 4);

		assert(t0->v == 6);
		assert(t1->v == 8);
		t1->v = 10;
		t0 = std::move(t1);
		assert(t0->v == 10);
	}
	{
		int v0 = 3;
		auto t0 = lazy_init::make_lazy_init<type1, int&>(v0);
		assert(v0 == 3);

		auto t1(std::move(t0));
		assert(t1->v == 6);
		assert(v0 == 6);
	}
}

/***************************************************************************/
