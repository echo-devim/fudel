#ifndef __FUDEL_H__
#define __FUDEL_H__

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "gui.h"
#include "cxxopts.hpp"
#include "sqlite_modern_cpp/hdr/sqlite_modern_cpp.h"

using namespace std;
using namespace sqlite;

class Fudel {
    private:
        string dbname;
    public:
        Fudel();
        bool addFile(string &path, int lifetime);
        bool removeFile(string &path);
        bool postponeFile(string &path, int lifetime);
        void showFiles();
        void checkFiles();
};

#endif
