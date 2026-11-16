//
// Created by ubril on 11/16/2026.
//

#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "file_manager.h"

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::string;
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

inline void Shell::run() {
    string line;
    while (running) {
        cout << fm.pwd() << ">" << std::flush;
        if (!getline(std::cin, line)) break;
        execute(line);
    }
}

void Shell::add(const string& name,
    size_t minArgs,
    const string& usage,
    function<void(const Args&)> fn) {
    commands[name] = Command{std::move(fn), minArgs, usage};
}

void Shell::registerCommands() {
    add("pwd", 0, "pwd", [this](const Args&) {
        cout << fm.pwd() << "\n";
    });

    add ("ls", 0, "ls [path]", [this](const Args& arg) {
        fm.ls(arg.empty() ? "." : arg[0]);
    });

    add ("cd", 1, "cd <path>", [this](const Args& arg) {
       if (!fm.cd(arg[0]))
           cout << "cd: no such directory: " << arg[0] << "\n";
    });

    add("mkdir", 1, "mkdir <path>", [this](const Args& arg) {
        if (!fm.mkdir(arg[0]))
            cout << "mkdir: cannot create '" << arg[0] << "\n";
    });

    add ("help", 0, "help", [this](const Args&) {
       for (const auto& pair : commands) {
           cout << " " << pair.second.usage << "\n";
       }
    });

    add("exit", 0, "exit", [this](const Args&) {
       running = false;
    });
}

void Shell::execute(const string& line) {
    std::istringstream in(line);
    string name;
    if (!(in >> name)) return;

    Args args;
    string word;
    while (in >> word) {
        args.push_back(word);
    }
    auto it = commands.find(name);
    if (it == commands.end()) {
        cout << "Unknown command: " << name << "\n";
        return;
    }

    const Command& cmd = it->second;
    if (args.size() < cmd.minArgs) {
        cout << "usage: " << cmd.usage << "\n";
        return;
    }
    cmd.run(args);
}

int main() {
    FileManager fm;
    Shell shell(fm);
    shell.run();
}