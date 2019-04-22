#pragma once
#include <cstddef>

enum ref_type { Read, Write };

struct ref_trace {
	size_t pos;
	ref_type type;
        ref_trace(){}
        ref_trace(ref_type type, size_t pos)
	{ this->pos = pos, this->type = type; }
};

// total memory size
const size_t mem_size = 1ul << 30;
// page size
const size_t pgsize = 1ul << 12;


