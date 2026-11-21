//
// Created by ubril on 11/10/2026.
//
#ifndef FILEMANAGER_FILE_MANAGER_H
#define FILEMANAGER_FILE_MANAGER_H
#include <sstream>
#include <vector>
#include "file_system.h"
#include "folder.h"

class FileManager : public FileSystem {
private:
    unique_ptr<Folder> root;
    Folder* cwd;
    Node* resolve(const string&) const;
    Folder* resolveDirectory(const string&) const;
    File* resolveFile(const string&) const;
    bool addFile(File &file);
    bool addFolder(Folder &folder);

public:
    FileManager() : root(new Folder("")), cwd(root.get()){}
    bool openFolder(const string&)  override;
    void closeFolder();
    File* newFile(const string&) const override;
    bool newFolder(const string&) const override;
    bool remove(const string&) override;
    string properties() const;
    bool cd(const string&);
    string showCurrentFolder() const override;
    bool copy(const string&, const string&, const string&) override;
    bool move(const string&, const string&, const string&) override;
    string list(const string&) const override;
    bool mkdir(const string&) const;
    string info() const;

    bool writeToFile(const string&, const string&) const override;
    string showFileContent(const string&) const override;
    string showInfo(const string&) const;
    //string showItemPath(const string& name) const override;
    Node* findItem(const string&) const override;
};

inline bool FileManager::remove(const string &path) {
    size_t slash = path.find_last_of('/');
    string parentPath = (slash == string::npos) ? "." :
        (slash == 0) ? "/" : path.substr(0, slash);

    string name = (slash == string::npos) ? path : path.substr(slash+1);
    if (name.empty() || name == "." || name == "..") return false;

    Folder *f = resolveDirectory(parentPath);
    f->remove(name);
    return true;
}

inline bool FileManager::writeToFile
    (const string &content, const string &path) const {
    File* f = resolveFile(path);
    f->writeToFile(content);
    return true;
}

inline string FileManager::showFileContent(const string &path) const {
    File* f = resolveFile(path);
    return f->getData();
}

inline string FileManager::showInfo(const string& path=".") const {
    Node* node = nullptr;
    if ((node = resolveDirectory(path)))
        return node->properties();
    if ((node = resolveFile(path)))
        return node->properties();
    return "";
}

inline Node* FileManager::findItem(const string &name) const {
    return cwd->find(name);
}

std::pair<string, string> splitPathName(const string&path) {
    size_t slash = path.find_last_of('/');
    string parentPath = (slash == string::npos) ? "." :
        (slash == 0) ? "/" : path.substr(0, slash);
     a
    string name = (slash == string::npos) ? path : path.substr(slash+1);
    if (name.empty() || name == "." || name == "..")
        name = "";
    return {name, parentPath};
}

inline File* FileManager::newFile(const string &path) const {
    auto pair = splitPathName(path);
    Folder* f = resolveDirectory(pair.second);
    return f->newFile(pair.first);
}

inline bool FileManager::addFile(File &file) {
    cwd->addFile(file);
    return true;
}
inline bool FileManager::addFolder(Folder &folder) {
    cwd->addFolder(folder);
    return false;
}

inline void FileManager::closeFolder() { cwd = cwd->closeFolder(); }

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

inline File* FileManager::resolveFile(const string &path) const {
    size_t slash = path.find_last_of('/');
    string parentPath = (slash == string::npos) ? "." :
        (slash == 0) ? "/" : path.substr(0, slash);

    string name = (slash == string::npos) ? path : path.substr(slash+1);
    if (name.empty() || name == "." || name == "..") return nullptr;

    Folder *f = resolveDirectory(parentPath);
    Node* node = f->find(name);
    return (node && !node->isFolder()) ? dynamic_cast<File*>(node) : nullptr;
}

inline bool FileManager::openFolder(const string &path)  {
    Folder* f = resolveDirectory(path);
    if (!f) return false;
    cwd = f;
    return true;
}

inline string FileManager::showCurrentFolder() const {
    if (cwd == root.get()) return "/";
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
    ///path = "*/" + path;
    path += (names.empty()) ? "/" : "";
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

inline string FileManager::list(const string& path = ".") const {
    Folder* f = resolveDirectory(path);
    if (!f)
        return "ls: no such directory";
    string content;
    for (const auto& pair: f->items) {
        //cout << (pair.second->isFolder() ? "/" : "") << pair.first << "\n";
        content += (pair.second->isFolder() ? "/" : "") + pair.first + "\n";
    }
    return content;
}

inline bool FileManager::newFolder(const string& path) const {
    auto slash = path.find_last_of('/');
    string parentPath = (slash == string::npos) ? "." :
        (slash == 0) ? "/" : path.substr(0, slash);

    string name = (slash == string::npos) ? path : path.substr(slash+1);
    if (name.empty() || name == "." || name == "..") return false;

    Folder* parent = resolveDirectory(parentPath);
    return parent && parent->newFolder(name);
}
#endif //FILEMANAGER_FILE_MANAGER_H