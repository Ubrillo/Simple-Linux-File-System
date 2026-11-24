//
// Created by ubril on 11/4/2026.
//

#ifndef FILEMANAGER_FOLDER_H
#define FILEMANAGER_FOLDER_H
#include <map>
#include <memory>
#include "file.h"
using std::map;
using std::unique_ptr;

class Folder : public Node{
    friend class FileManager;

private:
    map<const string, unique_ptr<Node>> items;

public:
    using Node::Node;
    Folder(const Folder&);
    Folder& operator=(const Folder&);
    size_t getSize() const;

    void addFile(File&);
    void addFolder(Folder&);
    File* newFile(const string&, const string&);
    Folder* newFolder(const string&);
    const map<const string, unique_ptr<Node>>& getItems() const;
    bool isFolder()  const override {return true;}
    bool remove(const string&);
    Folder* closeFolder() const;
    Node* find(const string&) const;
    string properties() const override;

};

inline void Folder::addFile(File &file) {
    unique_ptr<Node>file_ptr(new File(file));
    file_ptr->root = this;
    items.insert({file.name, std::move(file_ptr)});
}

inline void Folder::addFolder(Folder &folder) {
    unique_ptr<Node>folder_ptr(new Folder(folder));
    folder_ptr->root = this;
    items.insert({folder.name, std::move(folder_ptr)});
}

inline File* Folder::newFile(const string &name, const string& content="") {
    unique_ptr<Node>smp(new File(name, content, this));
    File* raw = dynamic_cast<File*>(smp.get());
    items[name] = std::move(smp);
    return raw;
}

inline Folder* Folder::newFolder(const string &name) {
    unique_ptr<Node>smp(new Folder(name, this));
    Folder* raw = dynamic_cast<Folder*>(smp.get());
    items[name] = std::move(smp);
    return raw;
}

inline bool Folder::remove(const string &name) {
    return items.erase(name);
}

inline Folder* Folder::closeFolder() const { return root; }

inline const map<const string, unique_ptr<Node>>& Folder::getItems() const  { return items; }

inline Node* Folder::find(const string& name) const {
    auto it = items.find(name); //TO-DO: check what iterator is returned into auto
    return it == items.end() ? nullptr : it->second.get();
}

inline Folder::Folder(const Folder &f):Node(f) {
    for (const auto& pair : f.getItems()) {
        Node* node = pair.second.get();
        if (node->isFolder()) {
            Folder *old = dynamic_cast<Folder*>(node);
            unique_ptr<Node>new_ptr(new Folder(*old));
            new_ptr->root = this;
            items[pair.first] = std::move(new_ptr);
        }
        else {
            const File *oldFile = dynamic_cast<File*>(node);
            unique_ptr<Node>new_ptr(new File(*oldFile));
            new_ptr->root = this;
            items[pair.first]  = std::move(new_ptr);
        }
    }
}

inline Folder& Folder::operator=(const Folder &rhs) {
    if (this == &rhs) return *this;
    Folder tmp(rhs);
    name = rhs.name;
    items.swap(tmp.items);
    for (auto& pair : tmp.items) {
        pair.second->root = this;
    }
    return *this;
}

inline string Folder::properties() const  {
    return  "name: " + name +
            "\nroot: " + (root && root->parent() ? root->name : "/") +
            "\npath: " + path() +
            "\nsize: " + std::to_string(items.size());
}


////////////////////////
inline string Node::path() const {
    //if (!root) return "*/";
    vector<string>names;
    names.push_back(name);


    for (const Folder* f = root; f && f->parent(); f = f->parent())
        names.push_back(f->name);

    if (names.empty())
        return "/";

    string path;
    for (vector<string>::reverse_iterator it = names.rbegin();
        it != names.rend();
        ++it) {
        path += "/" + *it;
    }
    return path;
}

inline  size_t Folder::getSize() const{
    return items.size();
}


inline string File::properties() const {
    return  "name: " + name +
        "\nroot: " + (root && root->parent() ? root->getName() : "/") +
        "\npath: " + path();
}
///////////////////
#endif //FILEMANAGER_FOLDER_H