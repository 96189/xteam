
#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include "component.h"
#include <vector>
#include <algorithm>

class Folder :public AbstractFile
{
public:
    Folder(std::string name)
    {
        setName(name);
    }
    virtual void showFileName()
    {
        std::cout << getName() << std::endl;
        for (auto it = files_.begin(); it != files_.end(); ++it)
        {
            std::cout << "-- " << (*it)->getName() << std::endl;
        }
    }
public:
    void addFile(AbstractFile* file)
    {
        files_.push_back(file);
    }
    void deleteFile(AbstractFile* file)
    {
        auto it = std::find(files_.begin(), files_.end(), file);
        if (it != files_.end())
        {
            files_.erase(it);
        }
    }
private:
    std::vector<AbstractFile*> files_;
};

#endif