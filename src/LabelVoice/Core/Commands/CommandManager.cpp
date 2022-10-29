#include "CommandManager.h"

CommandManager::CommandManager(QObject *parent) : QObject(parent) {
    historyIndex = 0;
    savedHistoryIndex = 0;
    recording = false;

    edited = false;
}

CommandManager::~CommandManager() {
}

void CommandManager::saveNow() {
    savedHistoryIndex = historyIndex;
    setEdited(false);
}

bool CommandManager::isEdited() const {
    return edited;
}

bool CommandManager::canUndo() const {
    return historyIndex != 0;
}

bool CommandManager::canRedo() const {
    return historyIndex != historyStack.size();
}

void CommandManager::undo() {
    execute(historyStack.at(historyIndex - 1), true);

    historyIndex--;
    setEdited(savedHistoryIndex != historyIndex);
}

void CommandManager::redo() {
    execute(historyStack.at(historyIndex), true);

    historyIndex++;
    setEdited(savedHistoryIndex != historyIndex);
}

void CommandManager::execute(const QSCommandRef &cmd, bool isUndo) {
    if (recording) {
        Q_ASSERT(!isUndo);
        recordList.append(cmd);
    }
    for (auto subscriber : qAsConst(subscribers)) {
        subscriber->execute({cmd}, isUndo);
    }
}

void CommandManager::execute(const LVCommandList &cmds, bool isUndo) {
    if (recording) {
        Q_ASSERT(!isUndo);
        recordList.append(cmds);
    }
    for (auto subscriber : qAsConst(subscribers)) {
        subscriber->execute(cmds, isUndo);
    }
}

void CommandManager::startRecord() {
    if (recording) {
        return;
    }
    recording = true;
}

void CommandManager::stopRecord(const QString &desc) {
    if (!recording) {
        return;
    }

    if (!desc.isEmpty()) {
        recordList.first()->desc = desc;
    }

    // Saved status doesn't exist
    if (savedHistoryIndex > historyIndex) {
        savedHistoryIndex = -1;
    }
    // Remove all operations after
    while (historyStack.size() > historyIndex) {
        historyStack.pop_back();
    }
    historyStack.append(recordList);
    historyIndex++;

    setEdited(savedHistoryIndex != historyIndex);

    recording = false;
    recordList.clear();
}

void CommandManager::addSubscriber(ICommandSubscriber *subscriber) {
    subscribers.insert(subscriber);
}

void CommandManager::removeSubscriber(ICommandSubscriber *subscriber) {
    subscribers.remove(subscriber);
}

void CommandManager::setEdited(bool edited) {
    bool org = this->edited;
    this->edited = edited;
    if (this->edited != org) {
        emit editStateChanged(this->edited);
    }
}
