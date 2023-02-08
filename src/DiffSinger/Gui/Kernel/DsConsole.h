#ifndef DSCONSOLE_H
#define DSCONSOLE_H

#include "QsConsole.h"
#include "DsGuiGlobal.h"
#include "QDspxModel.h"

#ifdef qCs
#undef qCs
#endif
#define qCs qobject_cast<DsConsole *>(QsCoreConsole::instance())

class DsConsolePrivate;

class DSGUI_API DsConsole : public QsConsole {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsConsole)
public:
    explicit DsConsole(QObject *parent = nullptr);
    ~DsConsole();

    void reloadStrings();

public:
    /**
     * @brief aboutApp  Show application information
     * @param parent    QWidget parent
     */
    void aboutApp(QWidget *parent = nullptr);

    /**
     * @brief openFile Open a file dialog and parse selected DSPX file
     * @param dspx     Output ref
     * @param parent   QWidget parent
     * @return true: success, false: failed/canceled
     */
    bool openFile(QDspxModel *dspx, QWidget *parent = nullptr);

    /**
     * @brief importFile Open a file dialog and parse selected XXX file
     * @param dspx     Output ref
     * @param parent   QWidget parent
     * @return true: success, false: failed/canceled
     */
    bool importFile(QDspxModel *dspx, QWidget *parent = nullptr);

protected:
    DsConsole(DsConsolePrivate &d, QObject *parent = nullptr);
};

#endif // DSCONSOLE_H
