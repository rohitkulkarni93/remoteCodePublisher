myVector C::getVector()
{
		return myVec;
}
int main(){
	C c;
	myVector myv = c.getVector(); // though myVector is in typetable but as this file don't have using namespace B, it won't depend on FileB
	return 0;
}