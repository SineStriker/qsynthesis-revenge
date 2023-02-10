#ifndef IDSPORTER_H
#define IDSPORTER_H

#include "Api/INamePlugin.h"
#include "DsGuiGlobal.h"
#include "QDspxModel.h"

class DSGUI_API IDsPorter : public INamePlugin {
    Q_OBJECT
public:
    explicit IDsPorter(QObject *parent = nullptr);
    ~IDsPorter();

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

public:
    /**
     * @brief ports Supported importing or exporting scenes
     * @return Flags, default to SupportedPort::All
     */
    virtual SupportedPorts ports() const;

    /**
     * @brief filterString A translated filter string used in file dialog
     * @return A string that matches this format: Sample Format(*.abc)
     */
    virtual QString filterString() const = 0;

    /**
     * @brief Convert an SVS project file to a dspx model
     *
     * @param filename Absolute path of input file
     * @param out Output model reference
     * @param args Other key-value arguments
     * @return true Success
     * @return false Failed
     */
    virtual bool load(const QString &filename, QDspxModel *out,
                      const QMap<QString, QVariant> &args = {}) = 0;

    /**
     * @brief Convert a dspx model to an SVS project file
     *
     * @param filename Absolute path of output file
     * @param in Input model
     * @param args Other key-value arguments
     * @return true Success
     * @return false Failed
     */
    virtual bool save(const QString &filename, const QDspxModel &in,
                      const QMap<QString, QVariant> &args = {}) = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(IDsPorter::SupportedPorts)

#define IDsPorter_IID "DiffSinger.Plugin.DsPorter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IDsPorter, IDsPorter_IID)
QT_END_NAMESPACE

#endif // IDSPORTER_H
