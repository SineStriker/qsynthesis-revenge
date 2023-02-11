#ifndef IDSPXPORTER_H
#define IDSPXPORTER_H

#include "Api/INamePlugin.h"
#include "DsGuiGlobal.h"
#include "QDspxModel.h"

class DSGUI_API IDspxPorter : public INamePlugin {
    Q_OBJECT
public:
    explicit IDspxPorter(QObject *parent = nullptr);
    ~IDspxPorter();

    enum SupportedPort {
        Import = 0x1,
        DragIn = 0x2,
        ExportClip = 0x4,
        ExportTrack = 0x8,
        ExportFile = 0x16,
        Export = ExportClip | ExportTrack | ExportFile,
        All = Import | DragIn | Export,
    };
    Q_DECLARE_FLAGS(SupportedPorts, SupportedPort)

    struct SupportedFormat {
        QString name;
        QStringList suffixes;
    };

    static QString categoryName();

public:
    /**
     * @brief Supported importing or exporting scenes
     * @return Flags, default to SupportedPort::All
     */
    virtual SupportedPorts ports() const;

    /**
     * @brief A translated filter string used in file dialog
     * @return A string that matches this format: Sample Format(*.abc)
     */
    virtual SupportedFormat format() const = 0;

    /**
     * @brief Convert an SVS project file to a dspx model
     *
     * @param filename Absolute path of input file
     * @param out Output model reference
     * @param parent Widget/Object parent for inner objects
     * @return true Success
     * @return false Failed
     */
    virtual bool load(const QString &filename, QDspxModel *out, QObject *parent = nullptr) = 0;

    /**
     * @brief Convert a dspx model to an SVS project file
     *
     * @param filename Absolute path of output file
     * @param in Input model
     * @param parent Widget/Object parent for inner objects
     * @return true Success
     * @return false Failed
     */
    virtual bool save(const QString &filename, const QDspxModel &in, QObject *parent = nullptr) = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IDspxPorter::SupportedPorts)

#define IDspxPorter_IID "DiffScope.Plugin.DspxPorter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IDspxPorter, IDspxPorter_IID)
QT_END_NAMESPACE

#endif // IDSPXPORTER_H
