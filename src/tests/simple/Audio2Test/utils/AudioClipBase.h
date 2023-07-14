//
// Created by Crs_1 on 2023/7/13.
//

#ifndef CHORUSKIT_AUDIOCLIPBASE_H
#define CHORUSKIT_AUDIOCLIPBASE_H

#include "buffer/IAudioSampleProvider.h"
#include <QScopedPointer>
#include <set>
template<class T>
struct AudioClipBase {
    qint64 position = -1;
    T *content;
    qint64 startPos;
    qint64 length;

    bool operator<(const AudioClipBase<T> &other) const {
        return position < other.position;
    }
};
template<class T>
struct AudioClipSeriesBase {
public:
    virtual bool addClip(const AudioClipBase<T> &clip) {
        int clipL = clip.position;
        int clipR = clip.position + clip.length;
        auto it = m_clips.lower_bound({clipL});
        if(it != m_clips.end() && it->position < clipR) return false;
        if(it != m_clips.begin()) {
            it--;
            if(it->position + it->length > clipL) return false;
        }
        m_clips.insert(clip);
        return true;
    }
    AudioClipBase<T> findClipAt(qint64 pos) const {
        auto it = findClipIt(pos);
        if(it == m_clips.end()) return {};
        return *it;
    }
    QList<AudioClipBase<T>> clips() const {
        return QList<AudioClipBase<T>>(m_clips.begin(), m_clips.end());
    }
    virtual bool removeClipAt(qint64 pos) {
        auto it = findClipIt(pos);
        if(it == m_clips.end()) return false;
        m_clips.erase(it);
        return true;
    }
    virtual void clearClips() {
        m_clips.clear();
    }
    qint64 effectiveLength() const {
        auto it = m_clips.crbegin();
        if(it == m_clips.crend()) return 0;
        return it->position + it->length;
    }
protected:
    std::set<AudioClipBase<T>> m_clips;
    std::set<AudioClipBase<T>>::const_iterator findClipIt(qint64 pos) const {
        auto it = m_clips.upper_bound({pos});
        if(it == m_clips.begin()) return m_clips.end();
        it--;
        if(it->position + it->length > pos) return it;
        else return m_clips.end();
    }
};
#endif // CHORUSKIT_AUDIOCLIPBASE_H
