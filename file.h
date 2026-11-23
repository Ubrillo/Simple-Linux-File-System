//
// Created by ubril on 11/4/2026.
//

#ifndef FILEMANAGER_FILE_H
#define FILEMANAGER_FILE_H
#include <utility>

#include "node.h"

class File : public Node {
    friend class Folder;
    friend class FileManager;

private:
    string data;

public:
    //using Node::Node;
    File(const string& name, string content="", Folder* root=nullptr):
        Node(name, root), data(std::move(content)) {}
    void writeToFile(const string &data) { this->data = data ;}
    bool isFolder() const override { return false; }
    string getData()  { return data; }
    string properties() const override;
};



#endif //FILEMANAGER_FILE_H