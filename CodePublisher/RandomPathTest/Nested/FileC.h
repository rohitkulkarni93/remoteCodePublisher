using namespace B;

std::function<int(int, int)> globalLambda = [=](int a, int b) -> int{return a + b;};
using Token = std::string;
class C{
public:
	myVector getVector();
private:
	myVector myVec; // myVector is defined in fileB
};
