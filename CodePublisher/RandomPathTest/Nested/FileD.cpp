DC::DC()
{
	std::cout << "Constructor of DC";
}
DC::~DC()
{
	std::cout << "Destructor of DC";
}
DC::methodDC(){
	std::cout<< _d._tok << " : "<<_d.id;	
}
int main(){
	A a;
	a.methodA();
	return 0;
}