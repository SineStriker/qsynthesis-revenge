#ifndef ACETREECONTROLLER_H
#define ACETREECONTROLLER_H

#include <QObject>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class AceTreeItem;

    class AceTreeControllerPrivate;

    class AceTreeControllerBuilder;

    class AceTreeControllerBuilderPrivate;

    class QSFRAMEWORK_API AceTreeController : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AceTreeController)
    public:
        explicit AceTreeController(const QString &name, QObject *parent = nullptr);
        ~AceTreeController();

    public:
        QString controllerName() const;
        AceTreeItem *treeItem() const;
        AceTreeControllerBuilder *builder() const;

    public:
        bool addChild(AceTreeController *c);
        bool removeChild(AceTreeController *c);
        bool removeChild(const QString &name);

        AceTreeController *child(const QString &name) const;

    public:
        bool isOperational() const;

        void setup(AceTreeItem *item);
        void clean();

    protected:
        virtual void preBuild();
        virtual void postBuild();

        virtual void setupImpl(AceTreeItem *item);
        virtual void cleanImpl();

    protected:
        AceTreeController(AceTreeControllerPrivate &d, const QString &name, QObject *parent = nullptr);

        QScopedPointer<AceTreeControllerPrivate> d_ptr;

        friend class AceTreeControllerBuilder;

        friend class AceTreeControllerBuilderPrivate;
    };

    class QSFRAMEWORK_API AceTreeControllerBuilder : public QObject {
        Q_OBJECT
    public:
        explicit AceTreeControllerBuilder(QObject *parent = nullptr);
        AceTreeControllerBuilder(const QString &rootBuilder, QObject *parent = nullptr);
        ~AceTreeControllerBuilder();

    public:
        QString rootBuilder() const;
        void setRootBuilder(const QString &id);

        AceTreeController *build(const QString &id) const;

        template <class T>
        inline T *build(const QString &id) const;

        template <class T>
        inline T *buildRoot() const;

    public:
        using Builder = std::function<AceTreeController *()>;

        bool addBuilder(const QString &id, const Builder &builder);
        bool addBuilder(const QString &id, const QMetaObject *metaObject);
        bool removeBuilder(const QString &id);
        bool hasBuilder(const QString &id) const;

    public:
        bool addSubBuilder(const QString &parentId, const QString &childId);
        bool removeSubBuilder(const QString &parentId, const QString &childId);
        bool hasSubBuilder(const QString &parentId, const QString &childId) const;

    private:
        AceTreeControllerBuilderPrivate *d;
    };

    template <class T>
    T *AceTreeControllerBuilder::build(const QString &id) const {
        static_assert(std::is_base_of<AceTreeController, T>::value, "T should inherit from QsApi::AceTreeController");
        return qobject_cast<T *>(build(id));
    }

    template <class T>
    T *AceTreeControllerBuilder::buildRoot() const {
        return build<T>(rootBuilder());
    }

}

#endif // ACETREECONTROLLER_H