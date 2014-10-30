
#include<iostream>
#include<fstream>
#include<stdexcept>
using namespace std;

class ARG_ERROR: exception{};
class FILE_ERROR: public runtime_error{
    public:
        using runtime_error::runtime_error;
};

int main(int argc, char *argv[]) try {
    if(argc != 2) throw ARG_ERROR{};
    ifstream fin(argv[1], ios::binary);
    if(!fin) throw FILE_ERROR(argv[1]);
    
    char buf[4096];
    while(fin){
        fin.read(buf, 4096);
        cout.write(buf, fin.gcount());
    }
    cout.put('\n');
}
catch(const ARG_ERROR&){
    cerr << "Argument Error.\n";
    cerr << "Usage: cat path\n";
}
catch(const FILE_ERROR &e){
    cerr << "File Error: " << e.what() << '\n';
}
catch(...){
    cerr << "Unknown Error Occurred.\n";
}

