/* 
 * File:   main.cpp
 * Author: tadvent
 *
 * Created on 2014年10月30日, 下午12:39
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

class ARG_ERROR{};
struct FILE_ERROR: runtime_error {
    using runtime_error::runtime_error;
};

/*
 * 
 */
int main(int argc, char** argv) try {

    if(argc != 3) throw ARG_ERROR{};
    
    ifstream fin(argv[1], ios::binary);
    if(!fin) throw FILE_ERROR{argv[1]};
    ofstream fout(argv[2], ios::binary|ios::trunc);
    if(!fout) throw FILE_ERROR{argv[2]};
    
    char buf[1024];
    while(fin){
        fin.read(buf, 1024);
        fout.write(buf, fin.gcount());
    }
}
catch(const ARG_ERROR &e){
    cerr << "Argument Error\n";
    cerr << "Usage: cp src dst\n";
}
catch(const FILE_ERROR &e){
    cerr << "File Error: " << e.what() << '\n';
}
catch(...){
    cerr << "Unknown Error Occured.\n";
}
