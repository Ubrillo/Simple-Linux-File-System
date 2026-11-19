//
// Created by ubril on 11/16/2026.
//

#ifndef FILEMANAGER_SHELL_H
#define FILEMANAGER_SHELL_H
#include <functional>
#include <sstream>
#include "file_manager.h"

using std::function;
using std::getline;
using std::size_t;

using Args = vector<string>;
struct Command {
    function<void(const Args&)> run;
    std::size_t minArgs;
    string usage;
};

class Shell {
private:
    FileManager& fm;
    bool running = true;
    map<string, Command> commands;

    void add(const string& name, size_t, const string&, function<void(const Args&)>);
    void registerCommands();
    void execute(const string& line);

public:
    explicit Shell(FileManager& fm): fm(fm) { registerCommands(); }
    Shell(const Shell&) = delete;
    Shell& operator=(const Shell&) = delete;
    void run();
};

void Shell::registerCommands() {
    add("pwd", 0, "pwd", [this](const Args&) {
        cout << fm.pwd() << endl;
    });

    add ("ls", 0, "ls [path]", [this](const Args& arg) {
        fm.ls(arg.empty() ? "." : arg[0]);
    });

    add ("cd", 1, "cd <path>", [this](const Args& arg) {
       if (!fm.cd(arg[0]))
           cout << "cd: no such directory: " << arg[0] << endl;
    });

    add("mkdir", 1, "mkdir <path>", [this](const Args& arg) {
        if (!fm.mkdir(arg[0]))
            cout << "mkdir: cannot create '" << arg[0] << endl;
    });
    
    add("copy", 3, "<item-name> <from-path> <to-path>", 
        [this](const Args& arg) {
            if (!fm.copy(arg[0], arg[1], arg[2]))
                cout << "copy aborted!" << endl;
        });
    
    add ("move", 3, "<item-name> <from path> <to path>",
        [this](const Args& arg) {
            if (!fm.move(arg[0], arg[1], arg[2]))
                cout << "move aborted" << endl;
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