//
// Created by ubril on 11/16/2026.
//
#include "shell.h"
#include <iomanip>

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
    string word, current;
    bool inQuotes = false;

    while (in >> word) {
        if (!inQuotes) {
            if (word.front() != '"') {
                args.push_back(word);
                continue;
            }
            inQuotes = true;
            current = word.substr(1);

        }else {
            current += ' ' + word;
        }

        if (!current.empty() && current.back() == '"') {
            current.pop_back();
            args.push_back(current);
            inQuotes = false;
        }
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