/* Memory Simulater.
 * record memory reference trace of Array & Element
 * multi thread safe.
 */

/*
 * feature to add : page table
 */

#pragma once

#include "../header/trace.h"
#include "../page/page_rp.h"
#include <vector>
#include <assert.h>
#include <mutex>
#include <iostream>

using std::vector;
using std::mutex;
using std::cout;
using std::endl;

char memory_pool[mem_size];
const char *pool = memory_pool;

class Simulater;

/* Interface for a element */

template<typename T>
class Element {
	
	// address of the byte 
	size_t pos;
	// simulater it belongs
	Simulater *sim;

public:

	Element()
	{}
	
	Element(Simulater *sim, size_t pos)
	{ this->pos = pos, this->sim = sim; }

	void ref(ref_type) const;

	Element& operator = (const Element &val)
	{ return *this = (T) val; }
	
	// count write.
	// triggered when the element is assigned.
	Element& operator = (const T &val) {
		ref(Write);
		T *pt = (T*) (pool + pos);
		*pt = val;
		return *this;
	}

	// count read.
	// triggered when the element is used
	operator T() const {
		ref(Read);
		T *pt = (T*) (pool + pos);
		return *pt;
	}

	void display()
	{
		cout << pos << ", size = " << sizeof(T) << endl;
	}
};

/* Simulate an array.
 * alloc an array of length n with sim->new_array<T>(n)
 */
template<typename T>
class Array {
	size_t start;
	size_t len;
	Simulater *sim;
public:
	Array()
	{}
	Array(Simulater *sim, size_t top, size_t len)
	{ this->sim = sim, this->start = top, this->len = len; }
	Element<T> operator [] (size_t pos) 
	{
		assert(pos < len);
		return Element<T>(sim, start + pos * sizeof(T));
	}
	void display()
	{
		cout << "Array@" << start << ", len = "
		     << len << endl;
	}
};

class Simulater {

	/* recorded trace file */ 
	vector<ref_trace> trace;

	/* get page id. */
	inline size_t get_page(size_t la)
	{ return la / pgsize; }

	/* get page head */
	inline size_t get_page_head(size_t la)
	{ return get_page(la) * pgsize; }

	/* heap top */
	size_t top;

	/* replacement algorithm you use */
	page_rp *rp_algo;
	bool record;

	/* mutex for read/write op */
	mutex mut_ref;
	/* mutex for memory alloc/free */
	mutex mut_mem;

	public:

	/* @rp_algo : page replacement algorithm 
	 * @record : if simulater record reference trace 
	 */
	Simulater(page_rp *rp_algo, bool record)
	{
		top = 0;
		this->rp_algo = rp_algo;
		this->record = record;
	}

	void reset()
	{
		top = 0;
		trace.clear();
	}
	
	/* alloc an array of length n 
	 * @len : length of array
	 */
	template<typename T>
	Array<T> new_array(size_t len)
	{
		mut_ref.lock();
		if (top + len * sizeof(T) > mem_size)
			assert(false);
		Array<T> nar(this, top, len);
		top += len * sizeof(T);
		mut_ref.unlock();
		return nar;
	}

	/* free an array 
	 * @array : the array to free
	 * do nothing now.
	 */
	template<typename T>
	void free_array(const Array<T> &array)
	{
		
	}

	void read(size_t pos)
	{ ref(Read, pos); }

	void write(size_t pos)
	{ ref(Write, pos); }

	void ref(ref_type type, size_t pos)
	{
		mut_mem.lock();
		if (record) {
			trace.push_back(ref_trace(type, pos));
		}
		if (type == Read) rp_algo->read(pos / pgsize);
		else rp_algo->write(pos / pgsize);
		mut_mem.unlock();
	}

	/* get trace file */
	const vector<ref_trace>& get_trace()
	{ return trace; }
};

template<typename T>
void Element<T>::ref(ref_type type) const
{
	assert(sim != nullptr);
	sim->ref(type, pos);
}

