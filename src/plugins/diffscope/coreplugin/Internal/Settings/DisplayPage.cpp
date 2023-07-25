#include "DisplayPage.h"

#include <QMDecoratorV2.h>

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QFontDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QVBoxLayout>

namespace Core {

    namespace Internal {

        DisplayPage::DisplayPage(QObject *parent) : ISettingPage("core.Display", parent) {
            m_widget = nullptr;
            setTitle([]() { return tr("Display"); });
            setDescription([]() { return tr("Display"); });
            loadFontSettings(font);
            //            qDebug() << "Loaded font " + font.toString();
        }

        DisplayPage::~DisplayPage() {
        }

        QString DisplayPage::sortKeyword() const {
            return "Display";
        }

        bool DisplayPage::matches(const QString &word) const {
            return ISettingPage::matches(word);
        }

        QWidget *DisplayPage::widget() {
            if (!m_widget) {
                auto getLabelFontStr = [this](QFont &font) {
                    auto family = font.family() + " ";
                    auto size = QString::number(font.pointSize()) + " ";
                    auto weight = getFontWeightStr(QFont::Weight(font.weight())) + " ";
                    auto italic = font.italic() ? QString("Italic") : QString("");
                    return family + size + weight + italic;
                };

                auto label = new QLabel();
                label->setText(getLabelFontStr(font));

                auto button = new QPushButton();
                button->setText("Pick a Font...");

                auto mainLayout = new QVBoxLayout;
                mainLayout->addWidget(label);
                mainLayout->addWidget(button);

                auto mainWidget = new QWidget;
                mainWidget->setLayout(mainLayout);

                connect(button, &QPushButton::clicked, this, [=]() {
                    bool ok;

                    auto getFont = [](bool *ok, const QFont &initial, QWidget *parent, const QString &title = {},
                                      QFontDialog::FontDialogOptions options = {}) {
                        /// auto f = QApplication::font();
                        // qApp->setFont(QFontDatabase::systemFont(QFontDatabase::TitleFont));

                        QFontDialog dlg(parent);
                        dlg.setOptions(options);
                        dlg.setCurrentFont(initial);
                        dlg.resize(640, 640);
                        if (!title.isEmpty())
                            dlg.setWindowTitle(title);

                        auto combos = dlg.findChildren<QComboBox *>();
                        for (auto combo : qAsConst(combos)) {
                            qDebug() << combo;
                            combo->setItemDelegate(new QStyledItemDelegate());
                        }

                        // qApp->setFont(f);

                        int ret = (dlg.exec() || (options & QFontDialog::NoButtons));
                        if (ok)
                            *ok = !!ret;
                        if (ret) {
                            return dlg.selectedFont();
                        } else {
                            return initial;
                        }
                    };

                    auto resultFont = getFont(&ok, font, mainWidget);
                    if (ok) {
                        font = resultFont;
                        label->setText(getLabelFontStr(font));
                    }
                });

                m_widget = mainWidget;
            }
            return m_widget;
        }

        bool DisplayPage::accept() {
            //            qDebug() << "[DisplayPage] On Accept";
            //            QApplication::setFont(selected_font);
            if (!saveFontSettings(font))
                return false;
            return true;
        }

        void DisplayPage::finish() {
            if (m_widget) {
                m_widget->deleteLater();
                m_widget = nullptr;
            }
        }

        bool DisplayPage::loadFontSettings(QFont &font) {
            auto appPath = QCoreApplication::applicationDirPath();
            auto jsonPath = QDir::cleanPath(appPath + QDir::separator() + QString("qtmediate.user.json"));
            if (!QFile::exists(jsonPath)) {
                // qDebug() << "\"qtmediate.user.json\" does not exist.";
                return false;
            }

            QJsonObject jsonObj;
            if (!loadJsonFile(jsonPath, &jsonObj))
                return false;

            if (!jsonObj.contains("AppFont")) {
                // qDebug() << "\"AppFont\" does not exist";
                return false;
            }

            auto objAppFont = jsonObj.value("AppFont").toObject();
            if (!objAppFont.contains("Family") || objAppFont.value("Family").toString().isEmpty())
                return false;

            if (!objAppFont.contains("Size") || objAppFont.value("Size").toInt() == 0)
                return false;

            font.setFamily(objAppFont.value("Family").toString());
            font.setPointSize(objAppFont.value("Size").toInt());
            if (objAppFont.contains("Weight"))
                font.setWeight(objAppFont.value("Weight").toInt());
            if (objAppFont.contains("Italic"))
                font.setItalic(objAppFont.value("Italic").toBool());

            return true;
        }

        bool DisplayPage::saveFontSettings(const QFont &font) {
            // Create qtmediate.user.json if not exist.
            auto appPath = QCoreApplication::applicationDirPath();
            auto jsonPath = QDir::cleanPath(appPath + QDir::separator() + QString("qtmediate.user.json"));
            if (!QFile::exists(jsonPath)) {
                // qDebug() << "\"qtmediate.user.json\" does not exist.";
                QFile file(jsonPath);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                QTextStream out(&file);
                out << "{}";
                file.close();
            }

            QJsonObject jsonObj;
            if (!loadJsonFile(jsonPath, &jsonObj))
                return false;

            if (jsonObj.contains("AppFont")) {
                auto objAppFont = jsonObj.value("AppFont").toObject();
                objAppFont["Family"] = font.family();
                objAppFont["Size"] = font.pointSize();
                objAppFont["Weight"] = font.weight();
                objAppFont["Italic"] = font.italic();
                jsonObj["AppFont"] = objAppFont;
            } else {
                QJsonObject objAppFont;
                objAppFont.insert("Family", font.family());
                objAppFont.insert("Size", font.pointSize());
                objAppFont.insert("Weight", font.weight());
                objAppFont.insert("Italic", font.italic());
                jsonObj.insert("AppFont", objAppFont);
            }

            saveJsonFile(jsonPath, jsonObj);

            return true;
        }
        bool DisplayPage::loadJsonFile(const QString &filename, QJsonObject *jsonObj) {
            // Deserialize json
            QFile loadFile(filename);
            if (!loadFile.open(QIODevice::ReadOnly)) {
                //                qDebug() << "Failed to open \"qtmediate.user.json\"";
                return false;
            }
            QByteArray allData = loadFile.readAll();
            loadFile.close();
            QJsonParseError err;
            QJsonDocument json = QJsonDocument::fromJson(allData, &err);
            if (err.error != QJsonParseError::NoError) {
                //                qDebug() << "Failed to deserialize \"qtmediate.user.json\"" << err.error;
                return false;
            }
            if (json.isObject()) {
                *jsonObj = json.object();
            }
            return true;
        }
        bool DisplayPage::saveJsonFile(const QString &filename, QJsonObject &jsonObj) {
            QJsonDocument document;
            document.setObject(jsonObj);

            auto byteArray = document.toJson(QJsonDocument::Compact);
            QString jsonStr(byteArray);
            QFile file(filename);
            file.remove();

            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug() << "Failed to write " + filename;
                return false;
            }
            //            QTextStream in(&file);
            //            in << jsonStr;
            file.write(jsonStr.toUtf8());

            file.close();
            return true;
        }
        QString DisplayPage::getFontWeightStr(const QFont::Weight &weight) {
            switch (weight) {
                case QFont::Thin:
                    // TODO: translation.
                    return "Thin";
                case QFont::ExtraLight:
                    return "ExtraLight";
                case QFont::Light:
                    return "Light";
                case QFont::Normal:
                    return "Normal";
                case QFont::Medium:
                    return "Medium";
                case QFont::DemiBold:
                    return "DemiBold";
                case QFont::Bold:
                    return "Bold";
                case QFont::ExtraBold:
                    return "ExtraBold";
                case QFont::Black:
                    return "Black";
                default:
                    return "";
            }
        }

    }

}