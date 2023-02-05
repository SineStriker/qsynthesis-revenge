#include "DsLocalData.h"

#include "QJsonFormatter.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

Q_SINGLETON_DECLARE(DsLocalData)

#define SCC(X, Y) static const char X[] = Y

SCC(SECTION_NAME_HOME, "home");
SCC(KEY_NAME_HOME_NAV_WIDTH_RATIO, "navWidthRatio");
SCC(KEY_NAME_HOME_WINDOW_RECT, "windowRect");

DsLocalData::DsLocalData() {
    construct();

    home.navWidthRatio = 0;
}

DsLocalData::~DsLocalData() {
}

bool DsLocalData::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "Record error.";
        return false;
    }

    QJsonObject objDoc = doc.object();

    QList<double> winRectTmp;

    {
        QJsonFormatter f;
        bool res = f.parse(objDoc,
                           f.OT({{
                               SECTION_NAME_HOME, //
                               f.OT({
                                   {KEY_NAME_HOME_NAV_WIDTH_RATIO, f.D(&home.navWidthRatio)}, //
                                   {KEY_NAME_HOME_WINDOW_RECT, f.DL(&winRectTmp)}             //
                               })                                                             //
                           }}));
        if (!res) {
            return false;
        }
    } // Allocated memories by formatter has been released

    if (home.navWidthRatio > 0 && home.navWidthRatio < 1 && winRectTmp.size() >= 4) {
        home.windowRect.setX(qAbs(winRectTmp[0]));
        home.windowRect.setY(qAbs(winRectTmp[1]));
        home.windowRect.setWidth(qAbs(winRectTmp[2]));
        home.windowRect.setHeight(qAbs(winRectTmp[3]));
    }

    return true;
}

bool DsLocalData::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QJsonObject objDoc{{
        SECTION_NAME_HOME,
        QJsonObject{{KEY_NAME_HOME_NAV_WIDTH_RATIO, home.navWidthRatio},
                    {KEY_NAME_HOME_WINDOW_RECT,
                     QJsonArray{
                         home.windowRect.x(),
                         home.windowRect.y(),
                         home.windowRect.width(),
                         home.windowRect.height(),
                     }}},
    }};

    QJsonDocument doc;
    doc.setObject(objDoc);

    file.write(doc.toJson());
    file.close();

    return true;
}
