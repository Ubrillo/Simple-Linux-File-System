//
// Created by ubril on 11/16/2026.
//
#include "shell.h"

inline void Shell::run() {
    string line;
    while (running) {
        cout << fs.showCurrentFolder() << ">" << std::flush;
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