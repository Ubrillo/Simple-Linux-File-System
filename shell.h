//
// Created by ubril on 11/16/2026.
//
#ifndef FILEMANAGER_SHELL_H
#define FILEMANAGER_SHELL_H
#include <functional>
#include <sstream>
#include <iostream>
#include "file_system.h"
#include "file_manager.h"

using std::cout;
using std::endl;
using std::function;

using Args = vector<string>;
struct Command {
    function<void(const Args&)> run;
    std::size_t minArgs;
    string usage;
};

class Shell {
private:
    FileSystem& fs;
    bool running = true;
    map<string, Command> commands;

    void add(const string& name, size_t, const string&, function<void(const Args&)>);
    void registerCommands();
    void execute(const string& line);

public:
    explicit Shell(FileManager& fm): fs(fm) { registerCommands(); }
    Shell(const Shell&) = delete;
    Shell& operator=(const Shell&) = delete;
    void run();
};

inline void Shell::registerCommands() {
    add("pwd", 0, "pwd",
    [this](const Args&) {
        cout << fs.showCurrentFolder() << endl;
    });

    add ("ls", 0, "ls [path]",
    [this](const Args& arg) {
        cout << fs.list(arg.empty() ? "." : arg[0]) << endl;
    });

    add ("cd", 1, "cd <path>",
    [this](const Args& arg) {
       if (!fs.openFolder(arg[0]))
           cout << "cd: no such directory: " << arg[0] << endl;
    });

    add("mkdir", 1, "mkdir <path>",
    [this](const Args& arg) {
        if (!fs.newFolder(arg[0]))
            cout << "mkdir: cannot create '" << arg[0] << endl;
    });
    
    add("copy", 2, "copy <from-path> <to-path>",
        [this](const Args& arg) {
            if (!fs.copy(arg[0], arg[1]))
                cout << "copy aborted!" << endl;
    });
    
    add ("move", 2, "move <from path> <to path>",
        [this](const Args& arg) {
            if (!fs.move(arg[0], arg[1]))
                cout << "move aborted!" << endl;
    });

    add ("wtf", 2, "wtf <message> <file-path>",
        [this](const Args& arg) {
           if (!fs.writeToFile(arg[0], arg[1]))
               cout << "write aborted!" << endl;
        });

    add ("info", 1, "info <item-path>",
        [this](const Args& arg) {
        cout << fs.showInfo(arg[0]) << endl;
    });

    add ("rm", 1, "rm <path>",
        [this](const Args& arg) {
            if (!fs.remove(arg[0]))
                cout << "remove operation aborted!" << endl;
        });

    add ("printf", 1, "print <path>",
        [this](const Args& arg) {
        cout<< fs.showFileContent(arg[0]) << endl;
    });

    add ("newfile", 1, "newfile <path>",
    [this](const Args& arg) {
            if (arg.size() == 2) {
                if (!fs.newFile(arg[0], arg[1]))
                    cout << "newfile aborted!" << endl;
            }
            else {
                if (!fs.newFile(arg[0]))
                    cout << "newfile aborted!" << endl;
            }
    });

    add ("help", 0, "help", [this](const Args&) {
       for (const auto& pair : commands) {
           cout << " " << pair.second.usage << endl;
       }
    });

    add("exit", 0, "exit", [this](const Args&) {
       running = false;
    });
}
#endif //FILEMANAGER_SHELL_H