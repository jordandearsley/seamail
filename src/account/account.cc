#include "account.h"
#include "../providers/emailProvider.h"
#include "../events/accountEvent.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

string Account::sentPath = "sent";
string Account::deletedPath = "deleted";
string Account::inboxPath = "inbox";

Account::Account(EmailProvider& provider):provider{provider}{}
Account::~Account(){ delete session; }

ostream& Account::serialize(ostream& out){
    nlohmann::json account;
    account["session"] << session;
    return out << account;
}

istream& Account::deserialize(istream& in) {
    nlohmann::json account;
    in >> account;
    session = new Session();
    account["session"] >> session;
    return in;
}

vector<string> Account::getAllFolderPaths(){
    return provider.getAllFolderPaths(session);
}

Folder Account::getFolderByPath(string path, string sort){ 
    return provider.getFolderByPath(session, path, sort);
}

Email Account::getEmailById(string id){
    return provider.getEmailById(session, id);
}

void Account::sendEmail(Email email){
    provider.sendEmail(session, email);
    shared_ptr<Event> e(new AccountEvent(FOLDER_CONTENTS_CHANGED, *this, sentPath));
    notifyAllObservers(e);
}

void Account::addThreadToFolder(string threadId, string folderPath){ 
    provider.addThreadToFolder(session, threadId, folderPath);
    shared_ptr<Event> e(new AccountEvent(FOLDER_CONTENTS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
}

void Account::removeThreadFromFolder(string threadId, string folderPath){
    provider.removeThreadFromFolder(session, threadId, folderPath);
    shared_ptr<Event> e(new AccountEvent(FOLDER_CONTENTS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
}

string Account::addFolder(string folderPath){
    string newPath = provider.addFolder(session, folderPath);
    shared_ptr<Event> e(new AccountEvent(ACCOUNT_FOLDERS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
    return newPath;
}

void Account::removeFolder(string folderPath){
    provider.removeFolder(session, folderPath);
    shared_ptr<Event> e(new AccountEvent(ACCOUNT_FOLDERS_CHANGED, *this, folderPath));
    notifyAllObservers(e);
}

bool Account::login(string emailAddress, string password){
    try{
        session = provider.getSession(emailAddress, password);
        return true;
    }catch(AuthenticationFailedException e){
        return false;
    }
}

void Account::logout(){ delete session; }

