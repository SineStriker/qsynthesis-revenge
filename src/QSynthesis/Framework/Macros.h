#ifndef MACROS_H
#define MACROS_H

// Cast to QWidget
#define Q_W(T) qobject_cast<QWidget *>(T)

#define Q_WARNING QMessageBox::warning
#define Q_ERROR QMessageBox::critical

#define Q_PROPERTY_DECLARE(Class, prop, Prop)                                                      \
private:                                                                                           \
    Q_PROPERTY(Class prop READ prop WRITE set##Prop NOTIFY prop##Changed)                          \
    Class m_##prop;                                                                                \
                                                                                                   \
public:                                                                                            \
    inline Class prop() const {                                                                    \
        return m_##prop;                                                                           \
    }                                                                                              \
                                                                                                   \
    inline void set##Prop(const Class &prop) {                                                     \
        m_##prop = prop;                                                                           \
        update();                                                                                  \
        emit prop##Changed();                                                                      \
    }                                                                                              \
                                                                                                   \
Q_SIGNALS:                                                                                         \
    void prop##Changed();                                                                          \
                                                                                                   \
private:

#define Q_LAYOUT_PROPERTY_DELCARE                                                                  \
private:                                                                                           \
    Q_PROPERTY(                                                                                    \
        QMargins layoutMargins READ layoutMargins WRITE setLayoutMargins NOTIFY layoutChanged)     \
    Q_PROPERTY(                                                                                    \
        QPixelSize layoutSpacing READ layoutSpacing WRITE setLayoutSpacing NOTIFY layoutChanged)   \
public:                                                                                            \
    inline QMargins layoutMargins() const {                                                        \
        auto m = this->layout();                                                                   \
        return m ? m->contentsMargins() : QMargins();                                              \
    }                                                                                              \
                                                                                                   \
    inline void setLayoutMargins(const QMargins &margins) {                                        \
        auto m = this->layout();                                                                   \
        m ? m->setContentsMargins(margins) : void(0);                                              \
        emit layoutChanged();                                                                      \
    }                                                                                              \
                                                                                                   \
    inline QPixelSize layoutSpacing() const {                                                      \
        auto m = this->layout();                                                                   \
        return QPixelSize(m ? m->spacing() : 0);                                                   \
    }                                                                                              \
                                                                                                   \
    inline void setLayoutSpacing(const QPixelSize &spacing) {                                      \
        auto m = this->layout();                                                                   \
        m ? m->setSpacing(spacing.value()) : void(0);                                              \
        emit layoutChanged();                                                                      \
    }                                                                                              \
                                                                                                   \
Q_SIGNALS:                                                                                         \
    void layoutChanged();                                                                          \
                                                                                                   \
private:

#define Q_STRCAT(A, B) A##B

#define Q_INNER_LAYOUT_PROPERTY_DELCARE_(CC, LL, m)                                                \
private:                                                                                           \
    Q_PROPERTY(QMargins LL##LayoutMargins READ LL##LayoutMargins WRITE                             \
                   set##CC##LayoutMargins NOTIFY LL##LayoutChanged)                                \
    Q_PROPERTY(QPixelSize LL##LayoutSpacing READ LL##LayoutSpacing WRITE                           \
                   set##CC##LayoutSpacing NOTIFY LL##LayoutChanged)                                \
public:                                                                                            \
    inline QMargins LL##LayoutMargins() const {                                                    \
        return m->contentsMargins();                                                               \
    }                                                                                              \
                                                                                                   \
    inline void set##CC##LayoutMargins(const QMargins &margins) {                                  \
        m->setContentsMargins(margins);                                                            \
        emit LL##LayoutChanged();                                                                  \
    }                                                                                              \
                                                                                                   \
    inline QPixelSize LL##LayoutSpacing() const {                                                  \
        return QPixelSize(m->spacing());                                                           \
    }                                                                                              \
                                                                                                   \
    inline void set##CC##LayoutSpacing(const QPixelSize &spacing) {                                \
        m->setSpacing(spacing.value());                                                            \
        emit LL##LayoutChanged();                                                                  \
    }                                                                                              \
                                                                                                   \
Q_SIGNALS:                                                                                         \
    void LL##LayoutChanged();                                                                      \
                                                                                                   \
private:

#define Q_INNER_LAYOUT_PROPERTY_DELCARE(CC, LL, m) Q_INNER_LAYOUT_PROPERTY_DELCARE_(CC, LL, m)

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

#define Q_FIND_PARENT_WIDGET(ParentClass, FunName) ParentClass *FunName() const;

#define Q_FIND_PARENT_WIDGET_DECLARE(Class, ParentClass, FunName)                                  \
    ParentClass *Class::FunName() const {                                                          \
        static QWidget *ptr = nullptr;                                                             \
        if (!ptr) {                                                                                \
            ptr = parentWidget();                                                                  \
            while (ptr && strcmp(ptr->metaObject()->className(),                                   \
                                 ParentClass::staticMetaObject.className())) {                     \
                ptr = ptr->parentWidget();                                                         \
            }                                                                                      \
        }                                                                                          \
        return qobject_cast<ParentClass *>(ptr);                                                   \
    }

#endif // MACROS_H
