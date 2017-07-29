#pragma once

#include "../Project1/FileFour.h"

using namespace File;

using AliasForA = A;

class B {
public:
	AliasForA a;

	void invokeA() {
		a.doSomething();
		globalFunction(a);
	}
};