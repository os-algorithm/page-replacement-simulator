/*
 * Driver For Hacker. 
 * add hack(algo, hacker) to test hack result.
 * output miss time & competitive ratio. 
 */

#include "sim_mem.cpp"

class hacker {
public:
	Simulater *sim;
	string name;
	virtual void main(Simulater *sim) = 0;
};

#include "hacker_certain.cpp"
#include "hacker_certain_ran.cpp"

class HackDriver {

	void hack(hacker &h, page_rp &algo);
	vector<int> mem_size;
	
public:

	/* add the physical page number you want to test */
	HackDriver()
	{
		mem_size.push_back(16);
		mem_size.push_back(64);
		mem_size.push_back(128);
		mem_size.push_back(256);
	}

	void hack_all(hacker &hacker_this)
	{
		cout << "--- Hackall : " << hacker_this.name << " ---" << endl;
		hack(hacker_this, rp_fifo_algo);
		hack(hacker_this, rp_lru_algo);
		hack(hacker_this, rp_clock_algo);
		hack(hacker_this, rp_marking_algo);
		hack(hacker_this, rp_ran_algo);
		cout << "-------------------------------" << endl;
	}

	void main();
} hack;

void HackDriver::hack(hacker &h, page_rp &algo)
{
	cout << "hacker : " << h.name <<  endl << "defender : " << algo.name << endl;
	Simulater sim;
	h.main(&sim);
	vector<ref_trace> trace = sim.get_trace();
	vector<size_t> pos;
	vector<char> opt;
	for (ref_trace t : trace) {
		pos.push_back(t.pos);
		opt.push_back(t.type == Read ? 'R' : 'W');
	}
	for (int size : mem_size) {
		algo.reset(size);
		for (ref_trace t : trace) {
			if (t.type == Read) algo.read(t.pos);
			else algo.write(t.pos);
		}
		cout << "> size = " << size
		     << ", miss = " << algo.miss
		     << ", competitive = " << 1.0*algo.miss/get_opt(trace.size(), 1 << 20, size, pos, opt)
		     << endl;
	}
}

#include "hack_list.cpp"
