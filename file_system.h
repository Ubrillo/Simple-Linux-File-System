//
// Created by ubril on 11/15/2026.
//

#ifndef FILEMANAGER_FILE_SYSTEM_H
#define FILEMANAGER_FILE_SYSTEM_H
#include "folder.h"

class FileSystem {

public:
    virtual ~FileSystem()  = default;

    virtual bool writeToFile(const string&, const string&) const = 0;

    virtual string  showFileContent(const string&) const = 0;

    virtual string showInfo(const string&) const = 0;

    //virtual string showItemPath(const string& name) const = 0;

    virtual bool remove(const string&) = 0;

    virtual Node* findItem(const string&) const = 0;

    virtual string list(const string&) const = 0;

    virtual File* newFile(const string&) const = 0;

    virtual bool newFolder(const string&) const = 0;

    virtual bool copy(const string&, const string&, const string&) = 0;

    virtual bool move(const string&, const string&, const string&) = 0;

    virtual string showCurrentFolder() const = 0;

    virtual bool openFolder(const string&) = 0;
};
#endif //FILEMANAGER_FILE_SYSTEM_H
