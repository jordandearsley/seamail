#ifndef FOLDER_VIEW_H
#define FOLDER_VIEW_H

#include "account/account.h"
#include "view/view.h"

class FolderView : public View {
    std::string watchingFolder;
    std::function<void(std::string)> threadChangeHandler;
    std::function<void(std::string, std::string, std::function<void(std::string)>)> requestInput;

    Folder cachedFolder;
    bool sortUnread = false;


    template<typename ReturnType>
    ReturnType threadEmailAccumulate(std::string threadId, ReturnType first, 
        std::function<ReturnType(ReturnType, Email)> binOp) const {
        auto thread = account.getThreadById(threadId);
        return std::accumulate(thread.emailIds.begin(), thread.emailIds.end(), first, binOp);
    }

    bool threadIsRead(std::string threadId) const {
        return threadEmailAccumulate<bool>(threadId, true, 
            [](bool acc, Email e){
                return acc && e.read;
            });
    }

    time_t threadLatestDate(std::string threadId) const {
        return threadEmailAccumulate<time_t>(threadId, (time_t) 0,
            [](time_t acc, Email e){
                return std::max(acc, e.dateTime);
            });
    }

    void updateCachedFolders() {
        cachedFolder = account.getFolderByPath(watchingFolder);
        std::function<bool(std::string,std::string)> predicateUnread = 
            [&](std::string threadA, std::string threadB){
                return threadIsRead(threadA) && !threadIsRead(threadB);
            };
        std::function<bool(std::string,std::string)> predicateDate = 
            [&](std::string threadA, std::string threadB){
                return threadLatestDate(threadA) > threadLatestDate(threadB);
            };
        sort(cachedFolder.threadIds.begin(), cachedFolder.threadIds.end(),
            sortUnread ? predicateUnread : predicateDate); 
    }

    Folder getFolder() const { return cachedFolder; }
    std::string getSelectedThreadId() const {
        return getFolder().threadIds.at(selectedThreadIndex);
    }

    size_t selectedThreadIndex = SIZE_MAX;

    void updateThreadIndex(size_t index) {
        size_t threads = getFolder().threadIds.size();
        if (threads < 1)
            selectedThreadIndex = SIZE_MAX;
        else if (index >= 0 && index < threads) {
            selectedThreadIndex = index;
            threadChangeHandler(getFolder().threadIds.at(selectedThreadIndex));
        }
        if (win) refresh();
    }
   public:
    FolderView(Account& account, std::string watchingFolder,
               std::function<void(std::string)> threadChangeHandler,
               std::function<void(std::string, std::string, std::function<void(std::string)>)>
                   requestInput)
        : View(account,2),
          watchingFolder(watchingFolder),
          threadChangeHandler(threadChangeHandler),
          requestInput{requestInput} {
        updateCachedFolders();
        updateThreadIndex(0);
    }
    void selectNextThread() { updateThreadIndex(selectedThreadIndex + 1); }
    void selectPreviousThread() { updateThreadIndex(selectedThreadIndex - 1); }
    void moveSelectedThread();

    void onResize() override;
    void onDraw(bool isActive) const override;
    void notify(std::shared_ptr<Event> event) override;
    bool onInput(int key) override;
};

#endif
