#include <bits/stdc++.h>
using namespace std;

#include "sim_mem.cpp"
#include "page_rp.cpp"
#include "rp_random.cpp"
#include "rp_marking.cpp"
#include "rp_fifo.cpp"
#include "rp_lru.cpp"
#include "rp_clock.cpp"
#include "driver.cpp"
#include "hacker_driver.cpp"

int main()
{
	init();
	// analyzer.analyze();
	hack.main();
	return 0;
}

