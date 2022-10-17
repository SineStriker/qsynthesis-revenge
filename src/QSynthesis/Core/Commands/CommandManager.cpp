#include "CommandManager.h"

CommandManager::CommandManager(QObject *parent) : QObject(parent) {
    historyIndex = 0;
    savedHistoryIndex = 0;
    recording = false;
}

CommandManager::~CommandManager() {
}

void CommandManager::undo() {
}

void CommandManager::redo() {
}

void CommandManager::execute(const QSCommand &cmd, bool isUndo) {
}

void CommandManager::execute(const QSCommandList &cmds, bool isUndo) {
}

void CommandManager::startRecord() {
    if (recording) {
        return;
    }
}

void CommandManager::stopRecord() {
    if (!recording) {
        return;
    }
}

void CommandManager::addSubscriber(ICommandSubscriber *subscriber) {
    subscribers.insert(subscriber);
}

void CommandManager::removeSubscriber(ICommandSubscriber *subscriber) {
    subscribers.remove(subscriber);
}
