#ifndef QSFRAMEWORK_MACROS_H
#define QSFRAMEWORK_MACROS_H

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

#define Q_FIND_PARENT_WIDGET(ParentClass, FunName) ParentClass *FunName() const;

#define Q_FIND_PARENT_WIDGET_DECLARE(Class, ParentClass, FunName)                                  \
    ParentClass *Class::FunName() const {                                                          \
        static QWidget *ptr = nullptr;                                                             \
        if (!ptr) {                                                                                \
            ptr = parentWidget();                                                                  \
            while (ptr && qstrcmp(ptr->metaObject()->className(),                                  \
                                  ParentClass::staticMetaObject.className())) {                    \
                ptr = ptr->parentWidget();                                                         \
            }                                                                                      \
        }                                                                                          \
        return qobject_cast<ParentClass *>(ptr);                                                   \
    }

#endif // QSFRAMEWORK_MACROS_H
