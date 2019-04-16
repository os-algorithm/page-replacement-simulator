/*
 * Memory Simulater.
 * Use `Array arr = sim->new_array(n)` 
 * to construct a array of length n.
 */
#pragma once

#include "../header/trace.h"
#include <vector>
#include <assert.h>

using std::vector;

const size_t mem_size = 1ul << 32;
const size_t pgsize = 1ul << 4;

class Simulater;

class Element {
	size_t pos;
	Simulater *sim;
public:
	Element()
	{}
	Element(Simulater *sim, size_t pos)
	{ this->pos = pos, this->sim = sim; }

	void ref(ref_type) const;

	// count write.
	Element& operator = (size_t val) {
		ref(Write);
		return *this;
	}

	// count read.
	operator size_t() const {
		ref(Read);
		return 0;
	}
};
	
class Array {
	size_t start;
	size_t len;
	Simulater *sim;
public:
	Array()
	{}
	Array(Simulater *sim, int top, int len)
	{ this->sim = sim, this->start = top, this->len = len; }
	Element operator [] (size_t pos) 
	{
		assert(pos < len);
		return Element(sim, pos);
	}
};

class Simulater {
		
	vector<ref_trace> trace;

	inline size_t get_page(size_t la)
	{ return la / pgsize; }

	inline size_t get_page_head(size_t la)
	{ return get_page(la) * pgsize; }

	size_t top;

public:

	Simulater()
	{
		top = 0;
	}

	Array new_array(size_t bytes)
	{
		Array nar(this, top, bytes);
		top += bytes;
		return nar;
	}

	void read(size_t pos)
	{ ref(Read, pos); }

	void write(size_t pos)
	{ ref(Write, pos); }
	
	void ref(ref_type type, size_t pos)
	{
		trace.push_back(ref_trace(type, pos));
	}

	const vector<ref_trace>& get_trace()
	{ return trace; }
};

void Element::ref(ref_type type) const
{
	assert(sim != nullptr);
	sim->ref(type, pos);
}
