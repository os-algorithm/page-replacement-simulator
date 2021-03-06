#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <cstring>
#include <set>
#include <cstdlib>
#include <queue>
#include "page_rp.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::set;
using std::queue;
using std::priority_queue;

void page_rp::swap_in(size_t pos)
{
	miss++;
	if (mem.size() < n) {
		pull++;
		mem[pos] = PTE_P;
	} else {
		size_t page = find_swap();
		map<size_t, int>::iterator iter = mem.find(page);
		assert(iter != mem.end());
		if (iter->second & PTE_D)
			push++;
		pull++;
		mem.erase(iter);
		mem[pos] = PTE_P;
	}
}

void page_rp::reset(int n)
{
	reset_hook(n); 
	this->n = n;
	mem.clear();
	miss = push = pull = 0;
}

// get mask of page 	
int page_rp::get_mask(size_t pos)
{
	assert(mem.count(pos));
	return mem[pos];
}

// set mask for page
void page_rp::set_mask(size_t pos, int mask)
{
	assert(mem.count(pos));
	mem[pos] = mask;
}

// write out a page pos and set PTE_D
// for clock algorithm
void page_rp::write_back(size_t pos)
{
	push++;
	mem[pos] &= ~PTE_D;
}

bool page_rp::inside(size_t pos)
{ return mem.count(pos); }

void page_rp::write(size_t pos)
{
	map<size_t, int>::iterator iter = mem.find(pos);
	write_hook(pos);
	if (iter != mem.end()) {
		iter->second |= PTE_A | PTE_D;
	} else {
		swap_in(pos);
	}
}

void page_rp::read(size_t pos)
{
	map<size_t, int>::iterator iter = mem.find(pos);
	read_hook(pos);
	if (iter != mem.end()) {
		iter->second |= PTE_A;
	} else {
		swap_in(pos);
	}
}
