#ifndef QSMACROS_H
#define QSMACROS_H

// Concat string
#define Q_STRCAT(A, B) A##B

// Traverse Directory
#define Q_D_EXPLORE(str)                                                                           \
    QFileInfoList d;                                                                               \
    {                                                                                              \
        QDir dir;                                                                                  \
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);                          \
        dir.setPath(str);                                                                          \
        d = dir.entryInfoList();                                                                   \
    }

// Singleton
#define Q_SINGLETON(T)                                                                             \
private:                                                                                           \
    static T *self;                                                                                \
                                                                                                   \
private:                                                                                           \
    T *construct() {                                                                               \
        Q_ASSERT(!self);                                                                           \
        self = this;                                                                               \
        return self;                                                                               \
    }                                                                                              \
                                                                                                   \
public:                                                                                            \
    static T *instance() {                                                                         \
        return self;                                                                               \
    }

#define Q_SINGLETON_DECLARE(T) T *T::self = nullptr;

// Default Codec
#define Q_CHARSET_INSTANCE                                                                         \
protected:                                                                                         \
    QTextCodec *m_codec;                                                                           \
    bool m_charsetDetermined;                                                                      \
                                                                                                   \
public:                                                                                            \
    QTextCodec *codec() const;                                                                     \
    void setCodec(QTextCodec *codec);                                                              \
                                                                                                   \
    bool charsetDetermined() const;

#define Q_CHARSET_STATIC                                                                           \
private:                                                                                           \
    static QTextCodec *s_codeForDefault;                                                           \
                                                                                                   \
public:                                                                                            \
    static QTextCodec *codeForDefault();                                                           \
    static void setCodeForDefault(QTextCodec *codec);

#define Q_CHARSET_INSTANCE_DECLARE(T)                                                              \
    QTextCodec *T::codec() const {                                                                 \
        return m_codec;                                                                            \
    }                                                                                              \
                                                                                                   \
    void T::setCodec(QTextCodec *codec) {                                                          \
        m_codec = codec;                                                                           \
    }                                                                                              \
                                                                                                   \
    bool T::charsetDetermined() const {                                                            \
        return m_charsetDetermined;                                                                \
    }

#define Q_CHARSET_STATIC_DECLARE(T)                                                                \
    QTextCodec *T::s_codeForDefault = nullptr;                                                     \
                                                                                                   \
    QTextCodec *T::codeForDefault() {                                                              \
        return s_codeForDefault;                                                                   \
    }                                                                                              \
                                                                                                   \
    void T::setCodeForDefault(QTextCodec *codec) {                                                 \
        s_codeForDefault = codec;                                                                  \
    }

#define Q_CHARSET                                                                                  \
    Q_CHARSET_INSTANCE                                                                             \
    Q_CHARSET_STATIC

#define Q_CHARSET_DECLARE(T)                                                                       \
    Q_CHARSET_INSTANCE_DECLARE(T)                                                                  \
    Q_CHARSET_STATIC_DECLARE(T)

#define Q_UNIQUE_ID(T)                                                                             \
public:                                                                                            \
    void initId() {                                                                                \
        m_id = ++s_idMax;                                                                          \
        s_idMap.insert(m_id, this);                                                                \
    }                                                                                              \
                                                                                                   \
    void destructId() {                                                                            \
        s_idMap.remove(m_id);                                                                      \
    }                                                                                              \
                                                                                                   \
    int id() const {                                                                               \
        return m_id;                                                                               \
    }                                                                                              \
                                                                                                   \
    static T *LookUp(int id);                                                                      \
                                                                                                   \
protected:                                                                                         \
    int m_id;                                                                                      \
                                                                                                   \
    static int s_idMax;                                                                            \
    static QMap<int, T *> s_idMap;

#define Q_UNIQUE_ID_DECLARE(T)                                                                     \
    int T::s_idMax = 0;                                                                            \
    QMap<int, T *> T::s_idMap;                                                                     \
    T *T::LookUp(int id) {                                                                         \
        auto it = s_idMap.find(id);                                                                \
        if (it == s_idMap.end()) {                                                                 \
            return nullptr;                                                                        \
        }                                                                                          \
        return it.value();                                                                         \
    }

#endif // QSMACROS_H