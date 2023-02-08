#include "CUndoHistory.h"

CUndoHistory::CUndoHistory(QObject *parent) : QObject(parent) {
    historyIndex = 0;
    savedHistoryIndex = 0;
    recording = false;

    edited = false;
}

CUndoHistory::~CUndoHistory() {
}

void CUndoHistory::saveNow() {
    savedHistoryIndex = historyIndex;
    setEdited(false);
}

bool CUndoHistory::isEdited() const {
    return edited;
}

bool CUndoHistory::canUndo() const {
    return historyIndex != 0;
}

bool CUndoHistory::canRedo() const {
    return historyIndex != historyStack.size();
}

void CUndoHistory::undo() {
    execute(historyStack.at(historyIndex - 1), true);

    historyIndex--;
    setEdited(savedHistoryIndex != historyIndex);
}

void CUndoHistory::redo() {
    execute(historyStack.at(historyIndex), true);

    historyIndex++;
    setEdited(savedHistoryIndex != historyIndex);
}

void CUndoHistory::execute(const CUndoCommandRef &cmd, bool isUndo) {
    if (recording) {
        Q_ASSERT(!isUndo);
        recordList.append(cmd);
    }
    for (auto subscriber : qAsConst(subscribers)) {
        subscriber->execute({cmd}, isUndo);
    }
}

void CUndoHistory::execute(const CUndoCommandList &cmds, bool isUndo) {
    if (recording) {
        Q_ASSERT(!isUndo);
        recordList.append(cmds);
    }
    for (auto subscriber : qAsConst(subscribers)) {
        subscriber->execute(cmds, isUndo);
    }
}

void CUndoHistory::startRecord() {
    if (recording) {
        return;
    }
    recording = true;
}

void CUndoHistory::stopRecord(const QString &desc) {
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

void CUndoHistory::addSubscriber(CUndoSubscriber *subscriber) {
    subscribers.insert(subscriber);
}

void CUndoHistory::removeSubscriber(CUndoSubscriber *subscriber) {
    subscribers.remove(subscriber);
}

void CUndoHistory::setEdited(bool edited) {
    bool org = this->edited;
    this->edited = edited;
    if (this->edited != org) {
        emit editStateChanged(this->edited);
    }
}
