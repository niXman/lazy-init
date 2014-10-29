
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
