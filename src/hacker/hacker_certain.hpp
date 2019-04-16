/* hack certain algorithms
 */
#include "hacker.h"
#include "sim_mem.hpp"

class certain_hacker : public hacker {

public:

        certain_hacker()
	{ name = "certain hacker"; }
	virtual void main(Simulater *sim)
	{
		this->sim = sim;
		size_t len = pgsize * 129;
		Array arr = sim->new_array(len);
		size_t last = 0;
		for (int i = 0; i < 1 << 20; i++) {
			arr[last] = 1;
			last = (last + pgsize) % len;
		}
	}
} hacker_certain;
