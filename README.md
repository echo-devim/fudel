# Future Deletion

Future Deletion (fudel) is a tool to schedule the deletion of files and directories. 
This can be useful to keep your files on your system for a certain number of days without have to worry to periodically clean up the system.
The tool is a command-line tool with a minimal GUI support and notification system.

The project is based on [sqlite_modern_cpp](https://github.com/SqliteModernCpp/sqlite_modern_cpp) wrapper library.

## Prerequisites
You need to install the following libraries to compile the project:
*  libnotify
*  gtkmm-3.0
*  C++17 compliant compiler
*  libsqlite3

## Usage
Show the help menu:
```
$ ./fudel -h
Future Deletion Program, delete files in the next days
Usage:
  Fudel [OPTION...]

  -h, --help          Show this help
  -g, --gui           Add gui support
  -s, --show          Show files in the database (output in JSON format)
  -c, --check         Check and remove aged files
  -n, --new arg       Add a new file that will be deleted after the specified
                      days (-a)
  -a, --age arg       Set the days to wait for the new file until the
                      deletion
  -p, --postpone arg  Increase lifetime of the specified file
  -r, --remove arg    Remove a file from the database (not from the disk)
```
The first time you run the tool, it'll create a local database file (called `files.db`) in the same path of the executable.

Add a new file to the database:
```
$ ./fudel -n /tmp/test.txt -a 10
```
In this way, we added the entry `/tmp/test.txt` in the database with associated its lifetime `10`.
Show the database content with the following command:
```
$ ./fudel -s                    
ID:14,AGE:1,PATH:/tmp/a.txt
ID:15,AGE:10,PATH:/tmp/test.txt
ID:16,AGE:1,PATH:/tmp/test2.txt
```
**Every time** you invoke `--check` parameter, fudel iterates over all the entries in the database decreasing the age. When the age is equal to 0, the file/directory is deleted.
Thus, if you schedule a crontab job each day to run `./fudel -c`, in this case this means that `age` are the days left before the deletion.

If you set the `--gui` option, when used in combination with `-n` and `-p`, it will show a GTK prompt where to insert the age.
The usage is intended to be integrated easier into other tools (e.g. a file manager).
For example, with Thunar, you can set a custom action executing: `./fudel -n '%f' -g`.