#include "MusicTime.h"
#include <QRegularExpression>
#include <QTextStream>

#include "MusicTimeManager_p.h"

namespace ScriptMgr::Internal {

    MusicTime::~MusicTime() {
    }
    bool MusicTime::setMbt(int measure, int beat, int tick) {
        return false;
    }
    int MusicTime::measure() const {
        return 0;
    }
    int MusicTime::beat() const {
        return 0;
    }
    int MusicTime::tick() const {
        return 0;
    }
    QString MusicTime::toString() const {
        QTextStream t;
        t << measure() + 1 << ":" << beat() + 1 << ":";
        t.setFieldWidth(3);
        t.setFieldAlignment(QTextStream::AlignRight);
        t.setPadChar('0');
        t << tick();
        return t.readAll();
    }
    bool MusicTime::fromString(const QString &str) {
        QRegularExpression rx(R"(^\s*(\d+)\s*[:\xff1a]?\s*(\d*)\s*[:\xff1a]?\s*(\d*)\s*$)");
        auto match = rx.match(str);
        if(!match.hasMatch()) return false;
        setMbt(match.captured(1).toInt() - 1, match.captured(2).toInt() - 1, match.captured(3).toInt());
        return true;
    }
    double MusicTime::msec() const {
        return 0;
    }
    bool MusicTime::fromMsec(double msec) {
        return false;
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager): m_manager(manager) {
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, int measure, int beat, int tick): MusicTime(manager) {
        setMbt(measure, beat, tick);
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, const QString &str): MusicTime(manager) {
        fromString(str);
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, double msec): MusicTime(manager) {
        fromMsec(msec);
    }

} // Internal