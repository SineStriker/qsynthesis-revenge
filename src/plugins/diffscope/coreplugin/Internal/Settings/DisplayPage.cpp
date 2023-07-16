#include "DisplayPage.h"

#include <QMDecoratorV2.h>

#include "QApplication"
#include "QDebug"
#include "QDir"
#include <QFontDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace Core {

    namespace Internal {

        DisplayPage::DisplayPage(QObject *parent) : ISettingPage("core.Display", parent) {
            m_widget = nullptr;
            setTitle([]() { return tr("Display"); });
            setDescription([]() { return tr("Display"); });
            loadFontSettings(font);
            // qDebug() << font;
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
                auto label = new QLabel();
                label->setText(font.family() + font.pointSize());

                auto button = new QPushButton();
                button->setText("Pick a Font...");

                auto mainLayout = new QVBoxLayout;
                mainLayout->addWidget(label);
                mainLayout->addWidget(button);

                auto mainWidget = new QWidget;
                mainWidget->setLayout(mainLayout);

                connect(button, &QPushButton::clicked, this, [=]() {
                    bool ok;
                    auto resultFont = QFontDialog::getFont(&ok, font, mainWidget);
                    if (ok) {
                        font = resultFont;
                        label->setText(font.family() + QString(font.pointSize()));
                        // qDebug() << font.family() << font.pointSize();
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
                jsonObj["AppFont"] = objAppFont;
            } else {
                QJsonObject objAppFont;
                objAppFont.insert("Family", font.family());
                objAppFont.insert("Size", font.pointSize());
                jsonObj.insert("AppFont", objAppFont);
            }

            saveJsonFile(jsonPath, jsonObj);

            return true;
        }
        bool DisplayPage::loadJsonFile(const QString &filename, QJsonObject *jsonObj) {
            // Deserialize json
            QFile loadFile(filename);
            if (!loadFile.open(QIODevice::ReadOnly)) {
                // qDebug() << "Failed to open \"qtmediate.user.json\"";
                return false;
            }
            QByteArray allData = loadFile.readAll();
            loadFile.close();
            QJsonParseError err;
            QJsonDocument json = QJsonDocument::fromJson(allData, &err);
            if (err.error != QJsonParseError::NoError) {
                // qDebug() << "Failed to deserialize \"qtmediate.user.json\"" << err.error;
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
                // qDebug() << "Failed to write json file";
                return false;
            }
//            QTextStream in(&file);
//            in << jsonStr;
            file.write(jsonStr.toUtf8());

            file.close();
            return true;
        }

    }

}