//
// Created by ubril on 11/4/2026.
//

#ifndef FILEMANAGER_FILE_H
#define FILEMANAGER_FILE_H
#include "node.h"

class File : public Node {
    friend class Folder;
    friend class FileManager;

private:
    string data;

public:
    using Node::Node;
    void writeToFile(const string &data){ this->data = data; }
    bool isFolder() const override { return false; }
};


#endif //FILEMANAGER_FILE_H