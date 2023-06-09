#ifndef CHORUSKIT_DSPXCONTROLLER_H
#define CHORUSKIT_DSPXCONTROLLER_H

#include <Controllers/AceTreeController.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class DspxRootControllerPrivate;

    class CORE_EXPORT DspxRootController : public QsApi::AceTreeController {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DspxRootController)
    public:
        Q_INVOKABLE explicit DspxRootController(QObject *parent = nullptr);
        ~DspxRootController();

    public:
        QString projectVersion() const;
        void setProjectVersion(const QString &projectVersion);

        QString projectAuthor() const;
        void setProjectAuthor(const QString &projectAuthor);

        QString projectName() const;
        void setProjectName(const QString &projectName);

    protected:
        void setupImpl(QsApi::AceTreeItem *item) override;
        void cleanImpl() override;

    signals:
        void projectVersionChanged(const QString &version);
        void projectAuthorChanged(const QString &author);
        void projectNameChanged(const QString &name);

    protected:
        DspxRootController(DspxRootControllerPrivate &d, QObject *parent = nullptr);
    };

}


#endif // CHORUSKIT_DSPXCONTROLLER_H
