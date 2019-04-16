cc = g++
prog = sim

sim: ran.o opt.o
	$(cc) ran.o opt.o -o sim -O2 -std=c++11
	cp sim ./bin/sim
ran.o: ./src/ran.cpp
	$(cc) -c ./src/ran.cpp -o ran.o -O2 -std=c++11
opt.o: ./src/opt.cpp
	$(cc) -c ./src/opt.cpp -o opt.o -O2 -std=c++11

clean:
	find . -name "*~" | xargs rm  -rf
	find . -name "*.aux" | xargs rm  -rf
	find . -name "*.log" | xargs rm  -rf
	find . -name "*.o" | xargs rm -rf
