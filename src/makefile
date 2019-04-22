cc = g++
prog = sim
vpath %.h ./header
VPATH = hacker:page:header:analyzer

hackers = hacker_driver.cpp hacker_certain.hpp hacker_certain_ran.hpp hacker_multi_sort.hpp sim_mem.hpp
pages = page_rp.cpp rp_fifo.hpp rp_lru.hpp rp_marking.hpp rp_clock.hpp rp_random.hpp
headers = trace.h page_rp.h
flags = -O2 -std=c++11 -lpthread

sim: driver.o opt.o page.o analyzer.o hacker.o $(headers)
	$(cc) driver.o opt.o page.o analyzer.o hacker.o -o sim $(flags)

analyzer.o: analyzer.cpp $(pages) $(headers)
	$(cc) -c ./analyzer/analyzer.cpp -o analyzer.o $(flags)

driver.o: driver.cpp
	$(cc) -c driver.cpp -o driver.o $(flags)

opt.o: opt.cpp
	$(cc) -c opt.cpp -o opt.o $(flags)

page.o: $(pages)
	$(cc) -c ./page/page_rp.cpp -o page.o $(flags)

hacker.o: $(hackers)
	$(cc) -c ./hacker/hacker_driver.cpp -o hacker.o $(flags)

clean:
	find . -name "*~" | xargs rm  -rf
	find . -name "*.aux" | xargs rm  -rf
	find . -name "*.log" | xargs rm  -rf
	find . -name "*.o" | xargs rm -rf
	find . -name "sim" | xargs rm -rf
