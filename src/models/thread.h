#ifndef _THREAD_H_
#define _THREAD_H_

#include <vector>
#include <iostream>
#include <string>

#include "../utility/serializable.h"

class Thread: public Serializable {

    std::ostream& serialize(std::ostream&) override;
    std::istream& deserialize(std::istream&) override;

public:
    std::string id;
    std::string title;

    std::vector<std::string> emailIds;
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

    Thread(std::string title, std::vector<std::string> emailIds = {});
    Thread();
    ~Thread();
};

#endif