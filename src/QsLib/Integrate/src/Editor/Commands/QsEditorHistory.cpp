#include "QsEditorHistory.h"

QsEditorHistory::QsEditorHistory(QObject *parent) : QObject(parent) {
    historyIndex = 0;
    savedHistoryIndex = 0;
    recording = false;

    edited = false;
}

QsEditorHistory::~QsEditorHistory() {
}

void QsEditorHistory::saveNow() {
    savedHistoryIndex = historyIndex;
    setEdited(false);
}

bool QsEditorHistory::isEdited() const {
    return edited;
}

bool QsEditorHistory::canUndo() const {
    return historyIndex != 0;
}

bool QsEditorHistory::canRedo() const {
    return historyIndex != historyStack.size();
}

void QsEditorHistory::undo() {
    execute(historyStack.at(historyIndex - 1), true);

    historyIndex--;
    setEdited(savedHistoryIndex != historyIndex);
}

void QsEditorHistory::redo() {
    execute(historyStack.at(historyIndex), true);

    historyIndex++;
    setEdited(savedHistoryIndex != historyIndex);
}

void QsEditorHistory::execute(const QsUndoCommandRef &cmd, bool isUndo) {
    if (recording) {
        Q_ASSERT(!isUndo);
        recordList.append(cmd);
    }
    for (auto subscriber : qAsConst(subscribers)) {
        subscriber->execute({cmd}, isUndo);
    }
}

void QsEditorHistory::execute(const QsUndoCommandList &cmds, bool isUndo) {
    if (recording) {
        Q_ASSERT(!isUndo);
        recordList.append(cmds);
    }
    for (auto subscriber : qAsConst(subscribers)) {
        subscriber->execute(cmds, isUndo);
    }
}

void QsEditorHistory::startRecord() {
    if (recording) {
        return;
    }
    recording = true;
}

void QsEditorHistory::stopRecord(const QString &desc) {
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

void QsEditorHistory::addSubscriber(QsCommandSubscriber *subscriber) {
    subscribers.insert(subscriber);
}

void QsEditorHistory::removeSubscriber(QsCommandSubscriber *subscriber) {
    subscribers.remove(subscriber);
}

void QsEditorHistory::setEdited(bool edited) {
    bool org = this->edited;
    this->edited = edited;
    if (this->edited != org) {
        emit editStateChanged(this->edited);
    }
}
