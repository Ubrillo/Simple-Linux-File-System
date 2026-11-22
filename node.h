//
// Created by ubril on 11/4/2026.
//

#ifndef FILEMANAGER_NODE_H
#define FILEMANAGER_NODE_H
#include <string>
#include <utility>
#include<vector>
using std::string;
using std::vector;

class Folder;

class Node {
    friend class FileManager;
    friend class Folder;
protected:
    string name;
    Folder* root = nullptr;

public:
    Node(const string &id, Folder* parent=nullptr):
        name(id), root(parent) {}
    virtual bool isFolder() const = 0;
    virtual string properties() const = 0;
    string getName() const { return name; };
    Folder* parent() const { return root; }
    string path() const;

    //destructor
    virtual ~Node() = default;
};
#endif //FILEMANAGER_NODE_H
