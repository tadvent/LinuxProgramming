/* 
 * File:   main.cpp
 * Author: tadvent
 *
 * Created on 2014年10月30日, 下午3:42
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <iomanip>
using namespace std;

struct FILE_ERROR: runtime_error{
    using runtime_error::runtime_error;
};

void cpuinfo(){
    const char filepath[] = "/proc/cpuinfo";
    ifstream fin(filepath);
    if(!fin.is_open()) throw FILE_ERROR(filepath);
    
    string line;
    int n_processor = 0;
    string procn("processor");
    string freq("cpu MHz");
    vector<string> freqs;
    while(fin.good()){
        getline(fin, line);
        if(line.compare(0, procn.length(), procn) == 0){
            ++n_processor;
        }
        else if(line.compare(0, freq.length(), freq) == 0){
            auto pos = line.find_last_of(':') + 2;
            auto len = line.length() - pos;
            freqs.emplace_back(line, pos, len);
        }
    }
    
    cout << "Number of CPUs: " << n_processor << '\n';
    cout << "Frequensies:\n";
    for(const auto &s: freqs){
        cout << "\t" << s << " MHz.\n";
    }
    cout.put('\n');
}

void meminfo(){
    const char filepath[] = "/proc/meminfo";
    ifstream fin(filepath);
    
    int mem_total = 0;
    int mem_free = 0;
    string name;
    while(fin >> name){
        if(name == string("MemTotal:")){
            fin >> mem_total;
        }
        else if(name == string("MemFree:")){
            fin >> mem_free;
        }
        if(mem_total > 0 && mem_free > 0)
            break;
    }
    
    cout << "Total Memory: " << mem_total << " kB.\n";
    cout << "Free Memory: " << mem_free << " kB.\n";
    cout << "Used percentage: "
            << fixed << setprecision(2)
            << ((mem_total - mem_free) * 100.0 / mem_total)
            << "%.\n";
}

/*
 * 
 */
int main(int argc, char** argv) {
    cpuinfo();
    meminfo();

    return 0;
}

