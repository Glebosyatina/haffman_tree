all: haffman
	del source.o haffman_tree.o
haffman: source.o haffman_tree.o
	g++ source.o haffman_tree.o -o haffman

source.o: source.cpp
	g++ -c source.cpp

haffman_tree.o: haffman_tree.cpp
	g++ -c haffman_tree.cpp

