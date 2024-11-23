#include "haffman_tree.h"
#include <iostream>
#include <fstream>
#include <cmath>

HaffmanTree::HaffmanTree(int col) {
	count = col;
}

int HaffmanTree::readInfo(const std::string& file_name) {

	std::cout << "readInfo\n";
	std::ifstream file(file_name, std::ios::binary);
	unsigned char read_byte;
	int col_variety = 0;//������� ������ ����
	for (int i = 0; i < 256; i++) {
		trees[i] = NULL;
		symbols[i] = trees[i];

	}

	for (int i = 0; i < count; i++) {
		file.read((char*)&read_byte, 1);
		//file.get(read_byte);
		if (trees[read_byte] == NULL) {
			trees[read_byte] = new Node;//��������� ����� ����, ������� ��� ���� ���� � � symbols ���� ���������� ����� ���� ����:)

			symbols[read_byte] = trees[read_byte];
		
			trees[read_byte]->byte = read_byte;//� ���� ���� ������ ��� �������
			col_variety++;			//����� ������� ������� ������ ����, ��� ���������� ������
			trees[read_byte]->p++;	//����������� ������� ���������� �����(���������� - 0)
			trees[read_byte]->left = NULL;
			trees[read_byte]->right = NULL;
			trees[read_byte]->parent = NULL;
		}
		else{
			trees[read_byte]->p++;
		}
		/*std::cout << read_byte << " - ";
		for (int i = 7; i >= 0; i--) {
			std::cout << ((read_byte >> i) & 1);
		}
		std::cout << '\n';*/
	}
	file.close();
	return col_variety;
}


void HaffmanTree::makeTree(int col) {
	//std::cout << "makeTree\n";
	if (col > 1) {
		int min1 = 200000000, min2 = 200000000;
		unsigned int n1 = 0, n2 = 0;		//������� ����������� �� ������� �������(�����)

		//���� ������ ���� ���� ������������� ������(����)
		for (int i = 0; i < 256; i++) {
			if (trees[i] != NULL && trees[i]->p < min1) {
				n1 = i; min1 = trees[i]->p;
			}
		}

		//���� ������ ���� ���� ������������� ������(����)
		for (int i = 0; i < 256; i++) {
			if (trees[i] != NULL && trees[i]->p < min2 && i != n1) {
				n2 = i; min2 = trees[i]->p;
			}
		}

		Node* tmp = new Node;

		tmp->left = trees[n1]; tmp->right = trees[n2];	//���������� ��� ����� ������ �������(�����)

		tmp->p = trees[n1]->p + trees[n2]->p;

		tmp->parent = NULL;
		trees[n1]->parent = tmp; trees[n2]->parent = tmp;
		trees[n1] = tmp; trees[n2] = NULL;	//���������� ��� ������ � ����

		makeTree(col - 1);
	}

}

std::string* HaffmanTree::show() {
	std::string* codes = new std::string[256];//���� ����� ����

	std::cout << "show\n";
	if (count == 1) {
		int i = 0;
		while (symbols[i] == NULL) {
			i++;
		}
		std::cout << symbols[i]->byte << "-" << 0;

	}
	else {
		for (int i = 0; i < 256; i++) {
			std::string code = "";
			Node* tmp = symbols[i];
			while (tmp != NULL && tmp->parent != NULL) {
				if (tmp == tmp->parent->left) {
					code = "0" + code;
				}
				else
					code = "1" + code;
				tmp = tmp->parent;
			}
			if (tmp != NULL) {
				std::cout << symbols[i]->byte << "-" << code << '\n';
				codes[i] = code;//���������� � ������ ����
			}

		}
	}
	return codes;
}

//������������ ������� ������ ��� ������ � ����
void HaffmanTree::make_array(int* result, int n) {
	for (int i = 0; i < 256; i++) {
		Node* tmp = symbols[i];
		if (tmp != NULL) {
			result[i] = tmp->p;
		}
		else {
			result[i] = 0;
		}
	}
}

//������ ����� � ����
void HaffmanTree::write_arch_bytes(std::string& file_name,std::ofstream& out_file, std::string* codes) {
	unsigned char read_byte;		//���� ��� ������
	int counter = 7;    //����� ������������� ����, ��� �� ���������� ��� ��������� 1 � ���
	char write_byte = 0;    //���� ��� ������

	std::ifstream in_file(file_name, std::ios::binary);
	
	while (in_file.read((char*)&read_byte, 1)) {//����� �� ������ � �����
	
		if (symbols[read_byte] != NULL) {//��� �������� ����� �������� ����� ������, ��� ��� ��� �����������

			int len = codes[read_byte].length();//����� ����
			//std::cout << '\t' << read_byte << '-' << codes[read_byte] << '\n';
			for (int index = 0; index < len; index++) {//������ ��� �����

				if (counter == -1) {//����� ���� ��� ������ �����
					counter = 7;    //��������� ����� ������������� ����
					///��� �������� ����� ������ ����
					/*for (int i = 7; i >= 0; i--) {
						int tmp = (write_byte >> i) & 1;
						std::cout << tmp;
					}
					std::cout << '\n';*/
					///
					out_file << write_byte;//����� ���� � ����
					write_byte = 0;
				}
				char bit = codes[read_byte][index];
				if (bit == '1') {

					switch (counter) {//� ����������� �� ���� ����� ����� ������������� ����, ��� � ������ 1
					case 7: write_byte = write_byte | 0b10000000; counter--; break;
					case 6: write_byte = write_byte | 0b01000000; counter--; break;
					case 5: write_byte = write_byte | 0b00100000; counter--; break;
					case 4: write_byte = write_byte | 0b00010000; counter--; break;
					case 3: write_byte = write_byte | 0b00001000; counter--; break;
					case 2: write_byte = write_byte | 0b00000100; counter--; break;
					case 1: write_byte = write_byte | 0b00000010; counter--; break;
					case 0: write_byte = write_byte | 0b00000001; counter--; break;
					}
					
				}
				else {//codes[read_byte][index] == '0'
					if (counter != -1) {
						counter--;
					}
					else {
						counter = 7;
						///��� �������� ����� ������ ����
						/*for (int i = 7; i >= 0; i--) {
							int tmp =  (write_byte >> i) & 1;
							std::cout << tmp;
						}
						std::cout << '\n';*/
						///
						out_file << write_byte;
						write_byte = 0;
					}
				}
			
			}
		}
	}
	//���� ����� �� ����� ����� � � ��� ������� �������� ��������� ���� ����� ���
	if (counter != 7) {
		out_file << write_byte;
		//��� �������� ����� ������ ����
		/*for (int i = 7; i >= 0; i--) {
			int tmp = (write_byte >> i) & 1;
			std::cout << tmp;
		}*/
		//std::cout << '\n';
		///
	}	
	in_file.close();
}



//-----------------DEARCHIVING------------------------------//
void HaffmanTree::readInfo_dearchive(int* arr) {
	std::cout << "Dearchiving\n";
	std::cout << "readInfo\n";

	for (int i = 0; i < 256; i++) {
		if (arr[i] != 0) {
			trees[i] = new Node;//��������� ����� ����, ������� ��� ���� ���� � � symbols ���� ���������� ����� ���� ����:)

			symbols[i] = trees[i];

			trees[i]->byte = i;//� ���� ���� ������ ��� �������

			trees[i]->p = arr[i];	//������� �����
			trees[i]->left = NULL;
			trees[i]->right = NULL;
			trees[i]->parent = NULL;
		}
		else {
			trees[i] = NULL;
			symbols[i] = trees[i];
		}
	}

}

void HaffmanTree::write_dearchiving_bytes(std::string& filename, std::ofstream& out_file) {
	std::cout << "write dearchiving bytes\n";
	std::ifstream in_file(filename, std::ios::binary);
	std::string str;
	std::getline(in_file, str);//������� ������ ������, ����� ������ � ������ �������������� ������
	unsigned char read_byte;		//���� ��� ������
	int cnt = 0;    //������� ������������ ������

	Node* root = NULL;
	//���� ������
	for (int i = 0; i < 256; i++) {
		if (trees[i] != NULL && trees[i]->parent==NULL) {
			root = trees[i];
			break;
		}
	}

	Node* tmp = root;

	while (in_file.read((char*)&read_byte, 1)){
		int counter = 7;    //����� ������������ ����
		//in_file.get(read_byte)) {
		//std::cout << "\nreading new byte\n";
		//���� �� ���������� �����
		while (counter > -1) {

			int bit = (read_byte >> counter) & 1; //��� 0 | 1
			//std::cout << bit ;
			if (bit == 0) {
				tmp = tmp->left;	//���� ��� 0 �� ���� ������
			}
			else {
				tmp = tmp->right;	//���� ��� 1 �� ���� �������
			}

			if ((tmp->left == NULL) && (tmp->right == NULL)) {	//����� �� ����� - ��������������� �����
				//std::cout <<  tmp->byte << '\n';
				out_file << tmp->byte;
				tmp = root;
				cnt++;
			}
			if (cnt == count) {	//����� �������� �� �� ���������� ���� ��� � ����������, �������
				break;
			}
			counter--;
		}
	}
	std::cout << "\nTotal bytes write: " << cnt << '\n';
}