/* 
 * File:   main.cpp
 * Author: tadvent
 *
 * Created on 2014年10月28日, 下午7:13
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    static option long_options[] = {
        {"all", optional_argument, NULL, 'a'},
        {"dir", no_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'h'},
        {"output", required_argument, NULL, 'o'},
        {0, 0, 0, 0}
    };
    
    int op, opidx = 0;
    string cmdname;
    while((op = getopt_long(argc, argv, "a::dho:",
            long_options, &opidx)) != -1)
    {
        switch(op)
        {
            case 'a':
                cout << "all\n";
                if(optarg){
                    cout << "\targ: " << optarg << '\n';
                }
                break;
            case 'd':
                cout << "dir\n";
                break;
            case 'h':
                cmdname.assign(argv[0]);
                cmdname = cmdname.substr(cmdname.find_last_of('/') + 1);
                cout << "Usage: ";
                cout << cmdname << " [-a|--all [arg]] [-d|--dir] [-o|--output <file>]\n";
                break;
            case 'o':
                cout << "Output file: " << optarg << '\n';
                break;
            default:
                //cerr << "Error Option: " << static_cast<char>(optopt) << '\n';
                break;
        }
    }
    if(optind < argc)
    {
        cout << "Unprocessed arguments:\n";
        while(optind < argc)
        {
            cout << argv[optind++] << '\n';
        }
    }
    return 0;
}

