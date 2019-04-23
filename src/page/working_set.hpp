#pragma once

class working_set : public page_rp {

	working_set()
	{ name = "working_set"; }

	virtual void reset_hook(size_t n)
	{ tick_tick = n; }

	virtual void find_swap(size_t pos)
	{
		
	}
};
