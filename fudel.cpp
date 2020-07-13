#include "fudel.h"

Fudel::Fudel() {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    dbname = string(homedir) + "/files.db";
    //Check if the database exists
    ifstream f(dbname);
    if (!f.good()) {
        try {
            database db(dbname);
            db <<
                "CREATE TABLE IF NOT EXISTS tbl_files ("
                "   id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "   age INT,"
                "   path TEXT"
                ");";
        } catch (exception& e) {
            cout << "Error: Cannot create database (" << e.what() << ")" << endl;
        }
    }
}

bool Fudel::addFile(string &path, int lifetime) {
    if (lifetime <= 0)
        return false;
    try {
        database db(dbname);
        db << "INSERT INTO tbl_files (age,path) VALUES (?,?);" << lifetime << path;
        return true;
    } catch (exception& e) {
        cout << "Error: Cannot add file into database (" << e.what() << ")" << endl;
        return false;
    }
}

bool Fudel::removeFile(string &path) {
    try {
        database db(dbname);
        db << "DELETE FROM tbl_files WHERE path = (?);" << path;
        return true;
    } catch (exception& e) {
        cout << "Error: Cannot remove file from database (" << e.what() << ")" << endl;
        return false;
    }
}

bool Fudel::postponeFile(string &path, int lifetime) {
    if (lifetime <= 0)
        return false;
    try {
        database db(dbname);
        db << "SELECT id, age, path FROM tbl_files" >>
        [&](int id, int age, string target_path) {
            if (target_path == path) {
                db << "UPDATE tbl_files SET age = (?) WHERE id = (?);" << (age+lifetime) << id;
            }
        };
    } catch (exception& e) {
        cout << "Error: Cannot remove file from database (" << e.what() << ")" << endl;
    }
    return false;
}

void Fudel::showFiles() {
    try {
        database db(dbname);
        db << "SELECT id, age, path FROM tbl_files" >>
        [&](int id, int age, string path) {
            cout << "ID:" << id << ",AGE:" << age << ",PATH:" << path << endl;
        };
        
    } catch (exception& e) {
        cout << "Error: Cannot read database (" << e.what() << ")" << endl;
    }
}

void Fudel::checkFiles() {
    try {
        database db(dbname);
        db << "SELECT id, age, path FROM tbl_files" >>
        [&](int id, int age, string path) {
            age--;
            if (age <= 0) {
                //lifetime expired, delete the file/directory
                int deleted_files = filesystem::remove_all(path);
                if (deleted_files == 0) {
                    cout << "Error: Cannot delete " << path << endl;
                }
                db << "DELETE FROM tbl_files WHERE id = (?);" << id;
                string msg = "Deleted " + path;
                if (!show_notification(msg)) {
                    cout << "Error: Cannot show notification" << endl;
                }
            } else {
                db << "UPDATE tbl_files SET age = (?) WHERE id = (?);" << age << id;
            }
        };
        
    } catch (exception& e) {
        cout << "Error: Cannot read database (" << e.what() << ")" << endl;
    }
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("Fudel", "Future Deletion Program, delete files in the next days");
    options.add_options()
    ("h,help", "Show this help")
    ("g,gui", "Add gui support")
    ("s,show", "Show files in the database (output in JSON format)")
    ("c,check", "Check and remove aged files")
    ("n,new", "Add a new file that will be deleted after the specified lifetime (-a)", cxxopts::value<std::string>())
    ("a,age", "Set the time to wait for the new file until the deletion", cxxopts::value<int>())
    ("p,postpone", "Increase lifetime of the specified file", cxxopts::value<std::string>())
    ("r,remove", "Remove a file from the database (not from the disk)", cxxopts::value<std::string>())
    ;
    try {
        auto result = options.parse(argc, argv);
        Fudel fudel;
        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }
        if (result.count("check"))
            fudel.checkFiles();

        if (result.count("new")) {
            int age;
            if (!result.count("age")) {
                if (result.count("gui")) {
                    AgeBox agebox;
                    age = agebox.show();
                } else {
                    cout << "Error: --age or --gui parameter must be specified" << endl;
                    return 1;
                }
            } else {
                age = result["age"].as<int>();
            }
            string target_file = result["new"].as<std::string>();
            fudel.addFile(target_file, age);
        }

        if (result.count("postpone")) {
            int age;
            if (!result.count("age")) {
                if (result.count("gui")) {
                    AgeBox agebox;
                    age = agebox.show();
                } else {
                    cout << "Error: --age or --gui parameter must be specified" << endl;
                    return 1;
                }
            } else {
                age = result["age"].as<int>();
            }
            string target_file = result["postpone"].as<std::string>();
            fudel.postponeFile(target_file, age);
        }

        if (result.count("remove")) {
            string target_file = result["remove"].as<std::string>();
            fudel.removeFile(target_file);
        }

        if (result.count("show"))
            fudel.showFiles();

    } catch(exception& e) {
        cout << "Error: Wrong command line arguments (" << e.what() << ")" << endl;
        return 1;
    }
}
