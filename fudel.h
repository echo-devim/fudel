#ifndef __FUDEL_H__
#define __FUDEL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "gui.h"
#include "cxxopts.hpp"
#include "sqlite_modern_cpp/hdr/sqlite_modern_cpp.h"

#define DB_NAME "files.db"

using namespace std;
using namespace sqlite;

class Fudel {
    public:
    Fudel();
    bool addFile(string &path, int days);
    bool removeFile(string &path);
    bool postponeFile(string &path, int days);
    void showFiles();
    void checkFiles();
};

#endif