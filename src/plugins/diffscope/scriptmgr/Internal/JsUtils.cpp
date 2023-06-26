#include "JsUtils.h"

#include <QMessageBox>

namespace ScriptMgr::Internal {

    static QMessageBox::StandardButton strToButton(const QString &str) {
        if(str == "Ok") return QMessageBox::Ok;
        if(str == "Save") return QMessageBox::Save;
        if(str == "SaveAll") return QMessageBox::SaveAll;
        if(str == "Open") return QMessageBox::Open;
        if(str == "Yes") return QMessageBox::Yes;
        if(str == "YesToAll") return QMessageBox::YesToAll;
        if(str == "No") return QMessageBox::No;
        if(str == "NoToAll") return QMessageBox::NoToAll;
        if(str == "Abort") return QMessageBox::Abort;
        if(str == "Retry") return QMessageBox::Retry;
        if(str == "Ignore") return QMessageBox::Ignore;
        if(str == "Close") return QMessageBox::Close;
        if(str == "Cancel") return QMessageBox::Cancel;
        if(str == "Discard") return QMessageBox::Discard;
        if(str == "Help") return QMessageBox::Help;
        if(str == "Apply") return QMessageBox::Apply;
        if(str == "Reset") return QMessageBox::Reset;
        if(str == "RestoreDefaults") return QMessageBox::RestoreDefaults;
        return QMessageBox::NoButton;
    }

    static QString buttonToStr(int button) {
        if(button == QMessageBox::Ok) return "Ok";
        if(button == QMessageBox::Save) return "Save";
        if(button == QMessageBox::SaveAll) return "SaveAll";
        if(button == QMessageBox::Open) return "Open";
        if(button == QMessageBox::Yes) return "Yes";
        if(button == QMessageBox::YesToAll) return "YesToAll";
        if(button == QMessageBox::No) return "No";
        if(button == QMessageBox::NoToAll) return "NoToAll";
        if(button == QMessageBox::Abort) return "Abort";
        if(button == QMessageBox::Retry) return "Retry";
        if(button == QMessageBox::Ignore) return "Ignore";
        if(button == QMessageBox::Close) return "Close";
        if(button == QMessageBox::Cancel) return "Cancel";
        if(button == QMessageBox::Discard) return "Discard";
        if(button == QMessageBox::Help) return "Help";
        if(button == QMessageBox::Apply) return "Apply";
        if(button == QMessageBox::Reset) return "Reset";
        if(button == QMessageBox::RestoreDefaults) return "RestoreDefaults";
        return "";
    }

    static QMessageBox::StandardButtons buttonStrToButtons(const QStringList &strList) {
        QMessageBox::StandardButtons buttons;
        for(const auto &str: strList) {
            buttons |= strToButton(str);
        }
        return buttons;
    }

    static QMessageBox::Icon strToIcon(const QString &str) {
        if(str == "Question") return QMessageBox::Question;
        if(str == "Information") return QMessageBox::Information;
        if(str == "Warning") return QMessageBox::Warning;
        if(str == "Critical") return QMessageBox::Critical;
        return QMessageBox::NoIcon;
    }

    QString jsMsgBox(QWidget *parent, const QString &title, const QString &message, const QString &icon, const QStringList &buttons, const QString &defaultButton) {
        QMessageBox msg(parent);
        msg.setStandardButtons(buttonStrToButtons(buttons));
        if(!defaultButton.isEmpty()) msg.setDefaultButton(strToButton(defaultButton));
        msg.setIcon(strToIcon(icon));
        msg.setWindowTitle(title);
        msg.setText(message);
        return buttonToStr(msg.exec());
    }
}