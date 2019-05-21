#pragma once

#include <string>
using std::string;

class working_set : public page_rp {
	
public:
	
	working_set()
	{
		memset(name, 0, sizeof name);
		strcpy(name, "working set");
	}

	virtual void reset_hook(size_t n)
	{
	        this->n = n * 10;
		tick_tick = this->n / 2;
	}

	virtual void tick()
	{
	        assert(tick_tick == n / 2);
		assert(mem.size() <= n);
		vector<size_t> del;
		for (auto &i : mem) {
			if (!(i.second & PTE_A))
				del.push_back(i.first);
			else i.second &= ~PTE_A;
		}
		for (auto i : del) {
			swap_out(i);
		}
	}
	
	virtual size_t find_swap(size_t pos)
	{
		assert(false);
		return 0;
	}
};

