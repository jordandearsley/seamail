#ifndef EMAILPROVIDER_H
#define EMAILPROVIDER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "../account/session.h"
#include "../models/email.h"
#include "../models/folder.h"
#include "../models/thread.h"
#include "../utility/serializable.h"

class EmailProvider : public Serializable {
protected:
    // email to folder paths
    std::unordered_map<std::string, std::vector<std::string>> accountFolderPaths;
    // "/email/path" to folders
    std::unordered_map<std::string, Folder> folders;
    // "/email/threadId" to threads
    std::unordered_map<std::string, Thread> threads;
    // "/email/emailId" to emails
    std::unordered_map<std::string, Email> emails;

    void serialize(nlohmann::json&) const override;
    void deserialize(const nlohmann::json&) override;

public:
    EmailProvider();
    virtual ~EmailProvider() = 0;
    virtual std::vector<std::string> getAllFolderPaths(Session& ctx) = 0;
    virtual Folder getFolderByPath(Session& ctx, std::string folderPath, std::string sort) = 0;
    virtual Email getEmailById(Session& ctx, std::string emailId) = 0;
    virtual void sendEmail(Session& ctx, Email email) = 0;
    virtual Thread getThreadById(Session& ctx, std::string threadId) = 0;
    virtual void addThreadToFolder(Session& ctx, std::string threadId, std::string folderPath) = 0;
    virtual void removeThreadFromFolder(Session& ctx, std::string threadId, std::string folderPath) = 0;
    virtual std::string addFolder(Session& ctx, std::string folderPath) = 0;
    virtual void removeFolder(Session& ctx, std::string folderPath) = 0;
    virtual Session getSession(std::string emailAddress, std::string password) = 0;
};

#endif
