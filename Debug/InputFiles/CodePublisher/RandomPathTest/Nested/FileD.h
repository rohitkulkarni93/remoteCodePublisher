typedef struct D {
   Token _tok; // declared in FileC
   int id;
}; 

class DC{
public:
	DC();
	~DC();
	void methodDC();
private:
	struct D _d;
};