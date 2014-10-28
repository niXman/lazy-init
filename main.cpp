
#include <lazy-init/lazy-init.hpp>

#include <iostream>
#include <cassert>

/***************************************************************************/

struct type0 {
	type0()
		:v{}
	{}

	int v;
};

struct type1 {
	type1(int &v)
		:v{v}
	{ v = v*2; }

	int &v;
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
		auto t = lazy_init::make_lazy_init<int, int>(3);
		assert(*t == 3);
	}
}

/***************************************************************************/
