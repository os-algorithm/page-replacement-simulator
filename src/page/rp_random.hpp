#pragma once

#include <cstring>
#include <cstdlib>
#include <assert.h>
#include <map>
#include "page_rp.h"

using std::map;

/* Random Replacement Algorithm
 * replace an random page in memory.
 */
class rp_ran : public page_rp {

	vector<size_t> vec;
	bool out;
	size_t pos;
	
public:

	rp_ran()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "random");
	}

	virtual void reset_hook(size_t n)
	{
		vec.clear();
		out = false;
	}
	
	virtual void swap_in_hook(size_t addr)
	{
		if (!out) {
			vec.push_back(addr);
		} else {
			out = false;
			vec[pos] = addr;
		}
	}
	
	virtual size_t find_swap(size_t current)
	{
		assert(mem.size() == n);
		size_t ran = rand()%n;
		out = true;
		pos = ran;
		return vec[ran];
	}
} ;
