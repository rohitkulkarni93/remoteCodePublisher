namespace B{
	void globalFuncB(){
		std::cout<< "Global function of namespace B";		
	}
	typedef std::vector<std::string> myVector;
	struct Bstruct{
		void methodB(){
			std::cout<<"Member function of struct B";
		}
		int b;
		std::string bstr;
	};
}