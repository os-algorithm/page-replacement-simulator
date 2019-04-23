/* hack certain algorithms
 * k-competitive
 */ 
class certain_hacker_ran : public hacker {

public:

	bool first_run;

	const static size_t len = 1 << 18;
	
        certain_hacker_ran()
	{ first_run = true, name = "random certain hacker"; }

	vector<size_t> vec;
	
	void gen_seqence();
	
	virtual void main(Simulater *sim)
	{
		if (first_run) {
			gen_seqence();
			first_run = false;
		}
		this->sim = sim;
		Array<char> arr = sim->new_array<char>(len);
		for (size_t i : vec)
			arr[i] = 1;
	}
} ;

void certain_hacker_ran::gen_seqence()
{
	vec.clear();
	size_t length = 1 << 20;
	for (size_t i = 0; i < length; i++)
		vec.push_back(rand() % len);

}
