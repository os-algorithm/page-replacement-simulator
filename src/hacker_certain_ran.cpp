/* hack certain algorithms
 * k-competitive
 */ 
class certain_hacker_ran : public hacker {

public:

        certain_hacker_ran()
	{ name = "random certain hacker"; }
	virtual void main(Simulater *sim)
	{
		this->sim = sim;
		size_t len = pgsize * 129;
		Array arr = sim->new_array(len);
		size_t last = 0;
		for (int i = 0; i < 1 << 20; i++) {
			arr[(rand() % 129) * pgsize] = 1;
		}
	}
} hacker_certain_ran;
