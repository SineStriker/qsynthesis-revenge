#ifndef DSCONSOLE_H
#define DSCONSOLE_H

#include "DsGuiGlobal.h"
#include "QDspxModel.h"
#include "QsConsole.h"

#ifdef qCs
#undef qCs
#endif
#define qCs qobject_cast<DsConsole *>(QMCoreConsole::instance())

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
     * @brief viewPlugins   Show plugin settings
     * @param parent        QWidget parent
     */
    void viewPlugins(QWidget *parent = nullptr);

    /**
     * @brief openFile Open a file dialog and parse selected DSPX file
     * @param dspx     Output ref
     * @param parent   QWidget parent
     * @return true: success, false: failed/canceled
     */
    bool openFile(QDspxModel *dspx, QWidget *parent = nullptr);

    // Load from filename and commit information to file manager
    bool openFile(QDspxModel *model, const QString &filename, QWidget *parent = nullptr);

    /**
     * @brief saveAsFile Open a file dialog and save DSPX file
     * @param dspx     Input file
     * @param parent   QWidget parent
     * @return true: success, false: failed/canceled
     */
    bool saveAsFile(const QDspxModel &dspx, const QString &nameHint, QWidget *parent = nullptr);

    // Save to filename and commit information to file manager
    bool saveFile(const QDspxModel &model, const QString &filename, QWidget *parent = nullptr);

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
