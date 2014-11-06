/* 
 * File:   main.cpp
 * Author: tadvent
 *
 * Created on 2014年10月31日, 下午10:20
 */

#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <getopt.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <iterator>

using namespace std;

////////////////////////////////////////////////////
// Exceptions:
struct ARG_ERROR: exception{
    const char* what() const noexcept override{
        return "Argument Error";
    }
};
struct STAT_ERROR: runtime_error{
    using runtime_error::runtime_error;
};
struct FILE_ERROR: runtime_error{
    using runtime_error::runtime_error;
};
struct DIR_ERROR: runtime_error{
    using runtime_error::runtime_error;
};
struct GRP_ERROR: runtime_error{
    using runtime_error::runtime_error;
    gid_t gid;
    GRP_ERROR(gid_t Gid)
        :runtime_error{to_string(Gid)}, gid(Gid){}
};
struct USR_ERROR: runtime_error{
    using runtime_error::runtime_error;
    uid_t uid;
    USR_ERROR(uid_t Uid)
        : runtime_error{to_string(Uid)}, uid(Uid){}
};


////////////////////////////////////////////////////
string mode2str(const int mode){
    string ret;
    switch(mode & S_IFMT){
        case S_IFSOCK:
            ret = "s";
            break;
        case S_IFLNK:
            ret = "l";
            break;
        case S_IFREG:
            ret = "-";
            break;
        case S_IFBLK:
            ret = "b";
            break;
        case S_IFDIR:
            ret = "d";
            break;
        case S_IFCHR:
            ret = "c";
            break;
        case S_IFIFO:
            ret = "p";
            break;
        default:
            ret = "?";
            break;
    }
    
    static const string mode_table[] = {
      "---", "--x", "-w-", "-wx",
      "r--", "r-x", "rw-", "rwx"
    };
    
    ret += mode_table[(mode & 0700) >> 6];
    ret += mode_table[(mode & 0070) >> 3];
    ret += mode_table[(mode & 0007) >> 0];
    
    if(mode & S_ISUID) ret[3] = 's';
    if(mode & S_ISGID) ret[6] = 's';
    if(mode & S_ISVTX) ret[9] = 't';
    
    return ret;
}

string uid2name(const uid_t uid){
    errno = 0;
    struct passwd *usrinfo = getpwuid(uid);
    if(usrinfo == NULL) throw USR_ERROR{uid};
    return string(usrinfo->pw_name);
}
string gid2name(const gid_t gid){
    errno = 0;
    struct group *grpinfo = getgrgid(gid);
    if(grpinfo == NULL) throw GRP_ERROR{gid};
    return string(grpinfo->gr_name);
}

void lsfile_long(const string &path){
    struct stat fileinfo;
    if(stat(path.c_str(), &fileinfo) != 0)
        throw FILE_ERROR(path);

    string timestr(ctime(&fileinfo.st_mtim.tv_sec));
    //strftime(timestr, 30, "%c", ctime(&fileinfo.st_mtim));
    cout << mode2str(fileinfo.st_mode)
            << setw(4) << fileinfo.st_nlink << ' '
            << setw(9) << left << uid2name(fileinfo.st_uid)
            << setw(9) << gid2name(fileinfo.st_gid)
            << setw(9) << right << fileinfo.st_size << ' '
            << setw(12) << timestr.substr(4, 12) << ' '
            << path << '\n';
}

void lsdir(string path, bool longfmt){
    DIR* dirp = opendir(path.c_str());
    if(dirp == NULL) throw DIR_ERROR(path);
    
    vector<string> item_paths;
    if(path.back() != '/') path.append("/");
    for(;;){
        dirent *diritem = readdir(dirp);
        if(diritem == NULL) break;
        item_paths.push_back(path + diritem->d_name);
    }
    
    sort(item_paths.begin(), item_paths.end());
    if(!longfmt){
        copy(item_paths.begin(), item_paths.end(),
                ostream_iterator<string>(cout, "\n"));
    }
    else{
        for(const string &item: item_paths){
            lsfile_long(item);
        }
    }
    closedir(dirp);
}


void ls(const string &path, bool longfmt = false){
    struct stat pathinfo;
    if(lstat(path.c_str(), &pathinfo) != 0)
        throw STAT_ERROR(path);
    if(S_ISDIR(pathinfo.st_mode)){
        lsdir(path, longfmt);
    }
    else{
        if(longfmt){
            lsfile_long(path);
        }
        else{
            cout << path << '\n';
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {

    char c;
    bool longfmt = false;
    while((c = getopt(argc, argv, "l")) != -1){
        switch(c){
            case 'l':
                longfmt = true;
                break;
        }
    }
    if(optind < argc){
        for(int i = optind; i < argc; ++i){
            ls(argv[i], longfmt);
        }
    }
    //if(argc != 2) throw ARG_ERROR{};
    //ls(argv[1], true);
    return 0;
}

