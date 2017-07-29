#include <string>
#include <stack>
#include <algorithm>
#include "TarjanAlgorithm.h"
#include "../Graph/Graph.h"

using namespace GraphLib;

typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;

// A recursive function that finds and prints strongly connected
// components using DFS traversal
// u --> The vertex to be visited next
// disc[] --> Stores discovery times of visited vertices
// low[] -- >> earliest visited vertex (the vertex with minimum
//             discovery time) that can be reached from subtree
//             rooted with current vertex
// *st -- >> To store all the connected ancestors (could be part
//           of SCC)
// stackMember[] --> bit/index array for faster check whether
//                  a node is in stack

void TarjanStrongComponentAlgorithm::
stronglyConnectedComponents()
{
}

// The function to do DFS traversal. It uses SCCUtil()
void  TarjanStrongComponentAlgorithm::invoke(Graph<string, string> graph)
{

}