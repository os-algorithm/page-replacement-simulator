/* Random Marking Algorithm
 * is log(k)-Competitve
 */

#pragma once
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include <set>
#include <iostream>
#include "page_rp.h"

using std::set;
using std::cin;
using std::cout;
using std::endl;

class rp_marking : public page_rp {

	vector<size_t> unmarked;
	vector<size_t> marked;
	size_t current;

public:

	virtual void reset_hook(int n)
	{
		unmarked.clear();
		marked.clear();
	}
	
	rp_marking()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "Random Marking");
	}

	virtual void swap_in_hook(size_t pos)
	{
		marked.push_back(pos);
		set_mask(pos, get_mask(pos) | PTE_A);
	}
	
	virtual size_t find_swap()
	{
		while (!unmarked.empty()) {
			size_t top = unmarked[unmarked.size()-1];
			if (get_mask(top) & PTE_A) {
				unmarked.pop_back();
				marked.push_back(top);
			} else break;
		}
		if (unmarked.empty()) { 
			swap(unmarked, marked);
			random_shuffle(unmarked.begin(), unmarked.end());
			for (size_t i : unmarked) {
				set_mask(i, get_mask(i) & (~PTE_A));
			}
		}
		assert(!unmarked.empty());
		size_t ret = unmarked[unmarked.size()-1];
		unmarked.pop_back();
		assert(inside(ret));
		return ret;
	}
	
} ;
