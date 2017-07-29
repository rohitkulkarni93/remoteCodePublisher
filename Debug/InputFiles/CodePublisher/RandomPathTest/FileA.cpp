A::A(){
	
std::cout << "Constructor of A";

}



A::~A(){
	
std::cout << "Destructor of A";

}



void A::methodA() {
	
	std::cout<<"Method of A";

}



int main(){
	

	A a;
	
	a.methodA();
	// following line is syntactically wrong but shown for output purpose only
	
	globalFuncB(); 
	// even though there is entry of this function in typetable, it won't be shown as we are not using namespace B in this file
	
	return 0;


}