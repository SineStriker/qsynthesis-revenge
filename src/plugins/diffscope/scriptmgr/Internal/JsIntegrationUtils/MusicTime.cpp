#include "MusicTime.h"

namespace ScriptMgr::Internal {

    MusicTime::~MusicTime() {
    }
    void MusicTime::setMbt(int measure, int beat, int tick) {
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
        return QString();
    }
    void MusicTime::fromString(const QString &str) {
    }
    double MusicTime::msec() const {
        return 0;
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager) {
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, int measure, int beat, int tick) {
    }
    MusicTime::MusicTime(ScriptMgr::Internal::MusicTimeManager *manager, const QString &str) {
    }

} // Internal