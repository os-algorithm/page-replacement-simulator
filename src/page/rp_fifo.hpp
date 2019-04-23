#pragma once
#include <queue>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include "page_rp.h"

using std::queue;

/* FIFO Replacement Algorithm
 * First-in-first-out, replace the "oldest" page.
 * May suffer from Belady's Anomaly. 
 */
class rp_fifo : public page_rp {

	size_t time;
	queue<size_t> reco;
	
public:

	virtual void reset_hook(size_t n)
	{
		time = 0;
		while (!reco.empty())
			reco.pop();
	}
	
	rp_fifo()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "FIFO");
	}

	virtual void swap_in_hook(size_t pos)
	{
		reco.push(pos);
	}
	
	virtual size_t find_swap(size_t current)
	{
		assert(!reco.empty());
		assert(mem.size() == n);
		assert(reco.size() == n);
		size_t t = reco.front(); reco.pop();
		return t;
	}
	
} ;
