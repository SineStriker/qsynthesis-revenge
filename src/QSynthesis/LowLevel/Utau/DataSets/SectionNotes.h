#ifndef SECTIONNOTES_H
#define SECTIONNOTES_H

#include "../Common/QLinkNote.h"

class SectionNotes : public QList<QLinkNote> {
public:
    SectionNotes();
    SectionNotes(const QList<QLinkNote> &notes);
    SectionNotes(QList<QLinkNote> &&notes);

    double initTempo() const;
    void setInitTempo(double initTempo);

    QString trackName() const;
    void setTrackName(const QString &trackName);

    void setData(QList<QLinkNote> &&notes);
    void setData(const QList<QLinkNote> &notes);

private:
    double m_initTempo;
    QString m_trackName;
};
#endif // SECTIONNOTES_H
