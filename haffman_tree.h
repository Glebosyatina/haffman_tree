#pragma once
#include <iostream>
#include <string>


class HaffmanTree {
private:
	struct Node {
		unsigned char byte;
		long long int p = 0;
		Node* left, * right, * parent;
	};

	Node* trees[256], * symbols[256];
	int count;	//для считывания байтов, сколько их всего

public:

	HaffmanTree(int col);

	int readInfo(const std::string&);

	void makeTree(int col);

	std::string* show();

	void make_array(int*, int n);
	void write_arch_bytes(std::string& ,std::ofstream&,std::string* codes);


	void readInfo_dearchive(int*);
	void write_dearchiving_bytes(std::string&, std::ofstream&);

};