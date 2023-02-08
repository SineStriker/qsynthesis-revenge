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
