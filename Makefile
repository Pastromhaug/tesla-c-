
all:
	g++ -o bin/main -I include -std=c++11  src/main.cpp src/parser.cpp src/equationNode.cpp
