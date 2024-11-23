#include <iostream>
#include <climits>
#include <fstream>
#include <ctime>
#include "haffman_tree.h"

std::string archive(std::string file_name) {

    int cnt = 0;//amount of byte in file

    std::ifstream in_file(file_name, std::ios::binary);  //для архивации
    std::ofstream out_file(file_name+".glebosyatina", std::ios::binary);    //куда пишем
    char byte;
    //std::cout << "Source file: ";

    while (in_file.get(byte)){
        //in_file.get(byte)) {
        cnt++;
        //std::cout << byte;
    }
    std::cout << '\n';

    in_file.close();
    std::cout << "Total bytes read: " << cnt << '\n';
    HaffmanTree tree(cnt);
    std::cout << "Archiving\n";
    int col_variety = tree.readInfo(file_name);
    tree.makeTree(col_variety);

    std::string* codes;
    codes = tree.show();

    int bytes[256];
    tree.make_array(bytes, 256);
    for (int i = 0; i < 256; i++) {
        out_file << bytes[i] << ' ';    //пишем частоты в файл
    }

    //пишем в файл коды символов
    out_file << '\n';
   
    std::cout << "Go to write bytes\n";
    tree.write_arch_bytes(file_name, out_file, codes);

    out_file.close();

    std::cout << "\nFile is archived!\n\n";

    return file_name + ".glebosyatina";
}

std::string dearchive(std::string filename) {
    std::ifstream in_file(filename, std::ios::binary);
    std::string out_name = "new_"+filename;
    out_name.resize(out_name.length()-13);//отсекаем .glebosyatina
    int bytes[256];
    int p;      //частоты
    int cnt = 0; //количество байт
    int col_variety = 0;    //количество разных байт

    //считываем массив частот
    for (int i = 0; i < 256; i++) {
        in_file >> p;
        bytes[i] = p;
        if (p != 0) {
            col_variety++;
            cnt += p;
        }
    }

    HaffmanTree tree(cnt);
    tree.readInfo_dearchive(bytes);
    tree.makeTree(col_variety);
    tree.show();

    in_file.close();
    std::ofstream out_file(out_name, std::ios::binary);

    tree.write_dearchiving_bytes(filename, out_file);
    out_file.close();
    return out_name;
}

int main() {
    
    std::cout << "\t\t\t\t-----Haffman Archivator-----\n";
    std::cout << "\t\t\t\t0 - archive\t1 - dearchive\n";
    int mode;
    std::cin >> mode;
    if (std::cin.fail()) {//при вводе строки
        std::cout << "Choose right mode!!!(0/1)\n";
        system("pause");
        return 1;
    }
    if (mode == 0) {
        std::string file_name;
        std::cout << "Enter name of file: ";
        std::cin >> file_name;
        long double beg = time(0);

        std::string archive_file = archive(file_name);


        long double end = time(0);
        std::cout << "Name of archived file: " << archive_file;
        std::cout << "\nTime to archive(dearchive) : " << (int)((end - beg) / 3600) << " hours, " << (int)((time(0) - beg) / 60) << " min, " << (int)(end - beg) % 60 << " seconds.\n";

    }
    else if (mode == 1) {
        std::string file_name;
        std::cout << "Enter name of file: ";
        std::cin >> file_name;
        long double beg = time(0);

        std::string dearchive_file = dearchive(file_name);

        long double end = time(0);
        std::cout << "Name of dearchived file: " << dearchive_file;
        std::cout << "\n\nTime to archive(dearchive): " << (int)((end - beg) / 3600) << " hours, " << (int)((time(0) - beg) / 60) << " min, " << (int)(end - beg) % 60 << " seconds.\n";
    }
    else {
        std::cout << "Choose right mode!!!(0/1)\n";
    }

    system("pause");

	return 0;
}


