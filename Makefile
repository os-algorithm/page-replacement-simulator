run: ./src/ran.cpp
	g++ ./src/ran.cpp -o ./bin/ran -O2 -std=c++11
	cp ./bin/ran ran
clean:
	find . -name "*~" | xargs rm  -rf
	find . -name "*.aux" | xargs rm  -rf
	find . -name "*.log" | xargs rm  -rf
