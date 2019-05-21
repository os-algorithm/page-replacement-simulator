/* Basic LIRS Algorithm
 * O(log n) per page fault
 */

#pragma once
#include <queue>
#include <map>
#include <vector>
#include "page_rp.h"

using std::priority_queue;
using std::map;
using std::vector;

class rp_lirs_th : public page_rp {

	struct node_info {
		size_t pos;
		size_t irr;
		friend bool operator < (const node_info &a, const node_info &b)
		{ return a.irr < b.irr; }
	};

	struct info {
		size_t last;
		size_t irr;
		set<node_info>::iterator pt;
	};
	
	map<size_t, info> info_map;
	set<node_info> lir;

	const static double hir_per = 0.1;

	size_t lir_size, hir_size;
	
public:

	rp_lirs_th()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "LIRS Basic");
	}

	virtual void reset_hook(size_t n)
	{
		while (!lir.empty())
			lir.pop();
		while (!hir.empty())
			hir.pop();
		info_map.clear();
		hir_size = ceil(n * hir_per);
		assert(hir_size < n);
		lir_size = n - hir_size;
	}

	virtual void read_hook(size_t pos)
	{

	}

	virtual void write_hook(size_t pos)
	{
		if (info_map.count(pos)) {
			info_map[pos] = (state) {ticks, ticks - info_map[pos].last};
		} else {
			info_map[pos] = (state) {ticks, -1};
		}		
	}

	virtual void swap_in_hook(size_t current)
	{
		
	}

	virtual size_t find_swap(size_t current)
	{
		
	}
};
