/* base class for hacker  */

class hacker {
public:
	Simulater *sim;
	string name;
	virtual void main(Simulater *sim) = 0;
};
