#pragma once
#ifndef TARJANALGORITHM

#endif // !TARJANALGORITHM

#include <iostream>
#include "../Graph/Graph.h"

using namespace GraphLib;
using namespace std;

class TarjanStrongComponentAlgorithm
{
public:
	void invoke(Graph<string, string>);
	
private:
	void stronglyConnectedComponents();
};