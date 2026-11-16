//
// Created by ubril on 11/10/2026.
//
#ifndef FILEMANAGER_FILE_MANAGER_H
#define FILEMANAGER_FILE_MANAGER_H
#include <iostream>
#include <sstream>
#include <vector>
#include "folder.h"

using std::cout;
using std::endl;
using std::vector;

class FileManager {
private:
    unique_ptr<Folder> root;
    Folder* cwd;
    Node* resolve(const string&) const;
    Folder* resolveDirectory(const string&) const;

public:
    FileManager() : root(new Folder("")), cwd(root.get()){}
    void openFolder(const string&);
    void closeFolder();
    File* newFile(const string&)const;
    Folder* newFolder(const string&) const;
    void addFile(File&) const;
    void addFolder(Folder&) const;
    void list() const ;
    void remove(const string&);
    string properties() const;
    bool cd(const string&);
    string pwd() const;
    bool copy(const string&, const string&, const string&);
    bool move(const string&, const string&, const string&);
};

inline void FileManager::openFolder(const string &name) {
    cwd = dynamic_cast<Folder*>(cwd->find(name));
}

inline void FileManager::list() const {
    for (const std::pair<const string, unique_ptr<Node>> &item : cwd->getItems()) {
        if (item.second->isFolder()) {
            cout << "/" + item.first << endl;
        }else {
            cout << item.first << endl;
        }
    }
}

inline Folder* FileManager::newFolder(const string &name) const { return cwd->newFolder(name); }

inline File* FileManager::newFile(const string &name) const { return cwd->newFile(name); }

inline void FileManager::addFile(File &file) const { cwd->addFile(file); }

inline void FileManager::addFolder(Folder &folder) const { cwd->addFolder(folder); }

inline void FileManager::closeFolder() { cwd = cwd->closeFolder(); }

inline void FileManager::remove(const string &name) { cwd->remove(name); }

inline string FileManager::properties() const { return cwd->properties(); }

inline vector<string> split(const string& path) {
    vector<string> parts;
    std::stringstream ss(path);
    string part;
    while (std::getline(ss, part, '/')) {
        if (!part.empty() && part != ".")
            parts.push_back(part);
    }
    return parts;
}

inline Node* FileManager::resolve(const string &path) const {
    Node* current = (!path.empty() && path[0]=='/') ? root.get() : cwd;
    for (const string &part : split(path)) {
        if (!current->isFolder()) return nullptr;
        Folder *fd = dynamic_cast<Folder*>(current);
        if (part == "..") {
            current = fd->parent() ? fd->parent() : fd;
        }else {
            current = fd->find(part);
            if (!current) return nullptr;
        }
    }
    return current;
}

inline Folder* FileManager::resolveDirectory(const string &path) const {
    Node* node = resolve(path);
    return (node && node->isFolder()) ? dynamic_cast<Folder*>(node) : nullptr;
}

inline bool FileManager::cd(const string &path) {
    Folder* f = resolveDirectory(path);
    if (!f) return false;
    cwd = f;
    return true;
}

inline string FileManager::pwd() const {
    if (cwd == root.get()) return "*/";
    vector<string>names;
    for (const Folder* f = cwd; f->parent(); f = f->parent()) {
        names.push_back(f->getName());
    }
    //names.push_back("home");
    string path;
    for (vector<string>::reverse_iterator it = names.rbegin();
        it!=names.rend();
        ++it) {
        path += "/" + *it;
    }
    path = "*" + path;
    return path;
}

inline bool FileManager::copy(const string& name, const string &from, const string &to) {
    Folder* current = resolveDirectory(from);
    Folder* destination = resolveDirectory(to);
    Node* load = current->find(name);
    if (current && destination && load) {
        if (!load->isFolder()) {
            File *f = dynamic_cast<File*>(load);
            destination->addFile(*f);
            return true;
        }
        Folder *f = dynamic_cast<Folder*>(load);
        destination->addFolder(*f);
        return true;
    }
    return false;
}

inline bool FileManager::move(const string& name, const string &from, const string &to) {
    Folder *current = resolveDirectory(from);
    Folder *destination = resolveDirectory(to);
    if (!current || !destination) return false;

    map<const string, unique_ptr<Node>>::iterator it = current->items.find(name);
    if (it == current->items.end()) return false;
    //if (destination->items.count(name)) return false; check for duplicate
    unique_ptr<Node> node = std::move(it->second);
    current->items.erase(it);
    node->root = destination;
    destination->items.insert({name, std::move(node)});
    return true;
}

#endif //FILEMANAGER_FILE_MANAGER_H