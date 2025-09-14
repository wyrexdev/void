#pragma once

#include <QObject>
#include <unordered_map>
#include <string>
#include <iostream>

class History : public QObject
{
    Q_OBJECT
public:
    explicit History(QObject *parent = nullptr) : QObject(parent) {}

    static std::string currenTab;
    static std::unordered_map<std::string, std::string> history;

    static void add(const std::string &uuid, const std::string &url)
    {
        history[uuid] = url;
        emit instance()->historyChanged(uuid, true);
    }

    static void remove(const std::string &uuid)
    {
        auto it = history.find(uuid);
        if (it != history.end()) {
            history.erase(it);
            emit instance()->historyChanged(uuid, false);
        }
    }

    static void setCurrentTab(const std::string &uuid) {
        currenTab = uuid;
        emit instance()->currentTabChanged(uuid);
    }
    
    static History* instance()
    {
        static History inst;
        return &inst;
    }

signals:
    void historyChanged(const std::string &uuid, bool added);
    void currentTabChanged(const std::string &uuid);
};