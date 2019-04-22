/*
 * Driver For Hacker. 
 * add hack(algo, hacker) to test hack result.
 * output miss time & competitive ratio. 
 * 
 * Create a hacker : 
 * - Define a class extends from hacker
 * - rewrite function void main(Simulater *sim)
 * --  sim is simulater for the hacker 
 * - add construct function to modify name
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
#include "hacker_multi_sort.hpp"

extern size_t get_opt(const vector<ref_trace>&, int);

class HackDriver {

	void hack(hacker &h, page_rp &algo);
	vector<int> mem_size;
	
	void test_memory(size_t memory_size)
	{
		assert(memory_size % pgsize == 0);
		mem_size.push_back(memory_size / pgsize);
	}
	
public:

	
	/* add the physical page number you want to test */
	HackDriver()
	{
		test_memory(1 << 19);
		test_memory(1 << 22);
		test_memory(1 << 25);
		test_memory(1 << 30);
	}

	void hack_all(hacker &hacker_this)
	{
		rp_marking rp_marking_algo;
		rp_fifo rp_fifo_algo;
		rp_lru rp_lru_algo;
		rp_clock rp_clock_algo;
		rp_ran rp_ran_algo;

		cout << "--- Hackall : " << hacker_this.name << " ---" << endl;
		hack(hacker_this, rp_marking_algo);
		hack(hacker_this, rp_fifo_algo);
		hack(hacker_this, rp_lru_algo);
		hack(hacker_this, rp_clock_algo);
		hack(hacker_this, rp_ran_algo);
		cout << "-------------------------------" << endl;
	}

	void main();
} hack;

/* hack algorithm algo with hacker h 
 * online test, multi-thread allowed
 */
void HackDriver::hack(hacker &h, page_rp &algo)
{
	cout << "hacker : " << h.name <<  endl << "defender : " << algo.name << endl;
	Simulater sim(&algo, false);
        for (int size : mem_size) {
		algo.reset(size);
		sim.reset();
		h.main(&sim);
		cout << "> page = " << size
		     << ", miss = " << algo.miss
		     << endl;
        }
}

/* main function for hacker driver
 * you can use hack_all(hacker) to hack all algorithms with hacker
 */
void HackDriver::main()
{
	// hack_all(hacker_certain);
	// hack_all(hacker_certain_ran);
	multi_sort_hacker hacker_multi_sort;
	hack_all(hacker_multi_sort);
}

void hacker_main()
{
	hack.main();
}

