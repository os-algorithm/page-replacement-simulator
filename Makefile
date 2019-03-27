run: ./src/ran.cpp
	g++ ./src/ran.cpp -o ./bin/ran -O2 -std=c++11
	cp ./bin/ran ran
clean:
	rm ./bin/*
	rm ./ran
	rm ./src/*.cpp~
	rm ./src/*.h~
