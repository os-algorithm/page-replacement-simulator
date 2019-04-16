/*
 * Driver For Hacker. 
 * add hack(algo, hacker) to test hack result.
 * output miss time & competitive ratio. 
 */

#include <vector>
#include <string>
#include <iostream>

#include "sim_mem.hpp"

using std::string;
using std::cin;
using std::cout;
using std::endl;

#include "../page/page_rp.h"
#include "../page/rp_random.hpp"
#include "../page/rp_lru.hpp"
#include "../page/rp_clock.hpp"
#include "../page/rp_fifo.hpp"
#include "../page/rp_marking.hpp"
#include "hacker_certain.hpp"
#include "hacker_certain_ran.hpp"

extern size_t get_opt(const vector<ref_trace>&, int);

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
		rp_marking rp_marking_algo;
		rp_fifo rp_fifo_algo;
		rp_lru rp_lru_algo;
		rp_clock rp_clock_algo;
		rp_ran rp_ran_algo;

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
	for (int size : mem_size) {
		algo.reset(size);
		for (ref_trace t : trace) {
			if (t.type == Read) algo.read(t.pos);
			else algo.write(t.pos);
		}
		cout << "> size = " << size
		     << ", miss = " << algo.miss
		     << ", competitive = " << 1.0*algo.miss/get_opt(trace, size)
		     << endl;
	}
}

void HackDriver::main()
{
	hack_all(hacker_certain);
	hack_all(hacker_certain_ran);
}

void hacker_main()
{
	hack.main();
}

