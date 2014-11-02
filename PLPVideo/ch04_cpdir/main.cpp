/* 
 * File:   main.cpp
 * Author: tadvent
 *
 * Created on 2014年10月31日, 上午12:59
 */

#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

struct ARG_ERROR{};
struct FILE_ERROR: runtime_error{
    using runtime_error::runtime_error;
};
struct DIR_ERROR: runtime_error{
    using runtime_error::runtime_error;
};

void cpfile(const string &src, const string &dst){
    static char buf[1024 * 16]; // 16kB
    ifstream fin(src, ios::binary);
    if(fin.fail()) throw FILE_ERROR{src};
    ofstream fout(dst, ios::binary);
    if(fout.fail()) throw FILE_ERROR(dst);
    while(fin){
        fin.read(buf, sizeof(buf));
        fout.write(buf, fin.gcount());
    }
}

void cpdir(string src, string dst){
    if(src.back() == '/')
        src.erase(src.end() - 1);
    if(dst.back() == '/')
        dst.erase(dst.end() - 1);
    
    DIR* pdir = opendir(src.c_str());
    if(pdir == NULL)
        throw DIR_ERROR{src};
    
    if(mkdir(dst.c_str(), 0775) == -1)
        throw DIR_ERROR(dst);
    for(;;){
        auto pentry = readdir(pdir);
        if(pentry == NULL)
            break;
        if(pentry->d_type == DT_DIR){
            if(string(pentry->d_name) == "."
                    || string(pentry->d_name) == "..")
                continue;
            cpdir(src + '/' + pentry->d_name,
                    dst + '/' + pentry->d_name);
        }
        else if(pentry->d_type == DT_REG){
            cpfile(src + '/' + pentry->d_name,
                    dst + '/' + pentry->d_name);
        }
        else{
            cerr << "Can't process file: " << src << '/' << pentry->d_name << '\n';
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) try {

    if(argc != 3){
        throw ARG_ERROR{};
    }
    cpdir(argv[1], argv[2]);
    return 0;
}
catch(const ARG_ERROR &e){
    cerr << "Usage: cpdir src dst\n";
}
catch(const FILE_ERROR &e){
    cerr << "Can't open file: " << e.what() << '\n';
}
catch(const DIR_ERROR &e){
    switch(errno){
        case EACCES:
            cerr << "Permission denied: " << e.what() << '\n';
            break;
        case EMFILE:
        case ENFILE:
            cerr << "Too many files opened: " << e.what() << '\n';
            break;
        case ENOENT:
            cerr << "Directory not exist: " << e.what() << '\n';
            break;
        case ENOTDIR:
            cerr << "This is not a directory: " << e.what() << '\n';
            break;
        case EEXIST:
            cerr << "The named file exists: " << e.what() << '\n';
            break;
        default:
            cerr << "Unknown error of directory: " << e.what() << '\n';
            break;
    }
}
catch(const exception &e){
    cerr << "STD ERROR: " << e.what() << '\n';
}
catch(...){
    cerr << "Unknown Error Occured.\n";
}
