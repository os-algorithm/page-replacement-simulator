#pragma once
#include <cstddef>
#include <map>
#include <unordered_map>

using std::map;
using std::unordered_map;

const int PTE_P = 1;
const int PTE_A = 2;
const int PTE_D = 4;
const int MAX_LEN = 1 << 22;
const int MAX_SIZE = 512;

/* Abstract Base Class for Page Replacement Algorithms
 * n : size of physical memory
 * mem : contains of physical memory, mem[i] is tag of page i, 
 *       recording the information about present bit(PTE_P), accesse bit(PTE_A) and dirty bit(PTE_D); 
 *       PTE_A, PTE_D, PTE_P are automatically setted.
 * miss, push, pull : counter for the number of page fault, swap-out and swap-in
 * virtual function find_swap() : find a valid page to swap out when a page fault occurs & no space remains
 * function read(pos) : implement an read opt on a page 
 * function write(pos): implement an write opt on a page 
 */ 
class page_rp {

	void swap_in(size_t pos);
	
public:

	void swap_out(size_t pos);
	
	size_t n;
	unordered_map<size_t, int> mem;
	char name[20];

	size_t miss;
	size_t push;
	size_t pull;
	size_t access;
	size_t ticks;
	size_t tick_tick;
	double mem_use;
	
	// init and set memory size to n
	void reset(size_t n);
	// find a page to swap out , page fault cause by pos, return page number
	virtual size_t find_swap(size_t pos) = 0;
	virtual void write_hook(size_t pos)
	{}
	virtual void read_hook(size_t pos)
	{}
        virtual void reset_hook(size_t n)
	{}
	virtual void swap_in_hook(size_t pos)
	{}
	// clock interrupte
	virtual void tick()
	{}
        
	// get mask of page 	
	int get_mask(size_t pos);
	// set mask for page
	void set_mask(size_t pos, int mask);
	// write out a page pos and set PTE_D
	// for clock algorithm
	void write_back(size_t pos);
	bool inside(size_t pos);
	void write(size_t pos);
	void read(size_t pos);
	void ticker();
};

