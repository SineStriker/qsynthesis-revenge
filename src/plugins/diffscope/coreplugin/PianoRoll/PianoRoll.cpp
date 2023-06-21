#include "PianoRoll.h"
#include "PianoRoll_p.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>

#include <CToolBar.h>
#include <CoreApi/ILoader.h>

#include "ICore.h"
#include "Internal/Widgets/PianoKeyWidget.h"

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::PianoRoll::" << (func) << "(): ").maybeSpace()

    static const char settingCatalogC[] = "IProjectWindow/PianoRoll";

    static const char floatingPanelsGroupC[] = "FloatingPanels";

    static const char pianoKeyWidgetC[] = "PianoKeyWidget";

    PianoRollPrivate::PianoRollPrivate() {
    }

    PianoRollPrivate::~PianoRollPrivate() {
        for (const auto &item : qAsConst(floatingPanels)) {
            item.panel->deleteLater();
            item.titleBar->deleteLater();
        }

        qDeleteAll(pianoKeyWidgets);
    }

    void PianoRollPrivate::init() {
        Q_Q(PianoRoll);

        m_canvas = new QsApi::SynthVSplitter();
        m_canvas->setObjectName("canvas");

        ICore::autoPolishScrollBars(m_canvas);

        m_pianoKeyContainer = new Internal::PianoKeyContainer();
        m_pianoKeyContainer->setObjectName("piano-container");

        m_sectionWidget = new Internal::SectionWidget();
        m_sectionWidget->setObjectName("section-widget");

        m_layout = new QGridLayout();
        m_layout->setMargin(0);
        m_layout->setSpacing(0);
        m_layout->addItem(new QSpacerItem(0, 0), 0, 0);
        m_layout->addWidget(m_sectionWidget, 0, 1);
        m_layout->addWidget(m_pianoKeyContainer, 1, 0);
        m_layout->addWidget(m_canvas, 1, 1);

        q->setLayout(m_layout);
    }

    void PianoRollPrivate::readSettings() {
        Q_Q(PianoRoll);

        if (m_canvas->count() > 0) {
            for (const auto &item : qAsConst(floatingPanels)) {
                q->setFloatingPanelState(item.key, PianoRoll::Hidden);
            }
        }

        auto settings = ILoader::instance()->settings();
        auto obj = settings->value(settingCatalogC).toObject();

        QJsonValue value;

        // Floating panels
        auto floatingPanelsStateArr = obj.value(floatingPanelsGroupC).toArray();
        for (auto it = floatingPanelsStateArr.begin(); it != floatingPanelsStateArr.end(); ++it) {
            if (!it->isObject())
                continue;

            auto stateObj = it->toObject();
            value = stateObj.value("key");
            if (!value.isString())
                continue;

            QString id = value.toString();

            value = stateObj.value("state");
            if (!value.isUndefined() && !value.isString())
                continue;

            int state = QMetaEnum::fromType<PianoRoll::FloatingPanelState>().keyToValue(value.toString().toLatin1());
            if (state >= 0) {
                q->setFloatingPanelState(id, static_cast<PianoRoll::FloatingPanelState>(state));
            }
        }

        // Piano key widget
        value = obj.value(pianoKeyWidgetC);
        q->setCurrentPianoKeyWidget(value.isString() ? value.toString() : Internal::DefaultPianoKeyWidget);
    }

    void PianoRollPrivate::saveSettings() const {
        auto settings = ILoader::instance()->settings();

        QJsonObject obj;

        // Floating panels
        QJsonArray floatingPanelsStateArr;
        for (const auto &item : qAsConst(floatingPanels)) {
            floatingPanelsStateArr.append(QJsonObject{
                {"key",   item.key                                                                   },
                {"state", QMetaEnum::fromType<PianoRoll::FloatingPanelState>().valueToKey(item.state)},
            });
        }
        obj.insert(floatingPanelsGroupC, floatingPanelsStateArr);

        // Piano key widget
        obj.insert(pianoKeyWidgetC, currentPianoKeyWidget);

        settings->insert(settingCatalogC, obj);
    }


    void PianoRollPrivate::setPianoKeyWidget_helper(IPianoKeyWidget *w) {
        // Remove old widget
        auto org = m_pianoKeyContainer->area();
        if (org) {
            org->deleteLater();
        }

        // Create new widget
        m_pianoKeyContainer->setArea(w);
    }

    PianoRoll::PianoRoll(QWidget *parent) : PianoRoll(*new PianoRollPrivate(), parent) {
    }

    PianoRoll::~PianoRoll() {
    }

    void PianoRoll::addPianoKeyWidgetFactory(const QString &key, IPianoKeyWidgetFactory *factory) {
        Q_D(PianoRoll);
        if (!factory) {
            myWarning(__func__) << "trying to add null factory";
            return;
        }
        if (d->pianoKeyWidgets.contains(key)) {
            myWarning(__func__) << "trying to add duplicated factory:" << key;
            return;
        }
        d->pianoKeyWidgets.insert(key, factory);
    }

    void PianoRoll::removePianoKeyWidgetFactory(const QString &key) {
        Q_D(PianoRoll);

        auto it = d->pianoKeyWidgets.find(key);
        if (it == d->pianoKeyWidgets.end()) {
            myWarning(__func__) << "panel does not exist:" << key;
            return;
        }

        d->pianoKeyWidgets.erase(it);

        // Need to remove its widget because the call may happen from a library unload
        if (d->currentPianoKeyWidget == key) {
            if (d->pianoKeyWidgets.contains(Internal::DefaultPianoKeyWidget))
                setCurrentPianoKeyWidget(Internal::DefaultPianoKeyWidget);
            else
                setCurrentPianoKeyWidget({});
        }
    }

    QStringList PianoRoll::pianoKeyWidgetKeys() const {
        Q_D(const PianoRoll);
        return d->pianoKeyWidgets.keys();
    }

    QString PianoRoll::currentPianoKeyWidgetKey() const {
        Q_D(const PianoRoll);
        return d->currentPianoKeyWidget;
    }

    QWidget *PianoRoll::currentPianoKeyWidget() const {
        Q_D(const PianoRoll);
        return d->m_pianoKeyContainer->area();
    }

    void PianoRoll::setCurrentPianoKeyWidget(const QString &key) {
        Q_D(PianoRoll);

        if (d->currentPianoKeyWidget == key) {
            return;
        }

        if (!key.isEmpty()) {
            auto fac = d->pianoKeyWidgets.value(key);
            if (!fac) {
                return;
            }
            d->setPianoKeyWidget_helper(fac->create(nullptr));
        } else {
            d->setPianoKeyWidget_helper(nullptr);
        }
        d->currentPianoKeyWidget = key;

        emit pianoKeyWidgetChanged(key);
    }

    IPianoKeyWidgetFactory *PianoRoll::pianoKeyWidgetFactory(const QString &key) const {
        Q_D(const PianoRoll);
        return d->pianoKeyWidgets.value(key);
    }

    QAbstractButton *PianoRoll::addFloatingPanel(const QString &key, QWidget *panel, QWidget *titleBar) {
        Q_D(PianoRoll);

        if (!panel) {
            myWarning(__func__) << "trying to add null panel";
            return nullptr;
        }
        auto it = d->floatingPanels.find(key);
        if (it != d->floatingPanels.end()) {
            myWarning(__func__) << "trying to add duplicated panel:" << key;
            return it->titleBar->titleButton();
        }

        auto bar = new Internal::FloatingTitleBar();
        bar->setTitleBar(titleBar);

        connect(bar->foldButton(), &QAbstractButton::toggled, this, [this, key](bool checked) {
            setFloatingPanelState(key, checked ? Normal : Folded); //
        });

        connect(bar->closeButton(), &QAbstractButton::clicked, this, [key, this]() {
            setFloatingPanelState(key, Hidden); //
        });

        d->floatingPanels.insert(key, {key, panel, bar, Hidden});

        return bar->titleButton();
    }

    void PianoRoll::removeFloatingPanel(const QString &key) {
        Q_D(PianoRoll);

        auto it = d->floatingPanels.find(key);
        if (it == d->floatingPanels.end()) {
            myWarning(__func__) << "panel does not exist:" << key;
            return;
        }

        it->titleBar->takeTitleBar();
        delete it->titleBar;

        d->floatingPanels.erase(it);
    }

    QStringList PianoRoll::floatingPanelKeys() const {
        Q_D(const PianoRoll);
        return d->floatingPanels.keys();
    }

    PianoRoll::FloatingPanelState PianoRoll::floatingPanelState(const QString &key) {
        Q_D(const PianoRoll);
        auto it = d->floatingPanels.find(key);
        if (it == d->floatingPanels.end()) {
            return Hidden;
        }
        return it->state;
    }

    void PianoRoll::setFloatingPanelState(const QString &key, PianoRoll::FloatingPanelState state) {
        Q_D(PianoRoll);
        auto it = d->floatingPanels.find(key);
        if (it == d->floatingPanels.end()) {
            return;
        }

        auto &block = it.value();
        if (state == block.state) {
            return;
        }

        auto foldBtn = block.titleBar->foldButton();
        if (block.state == Hidden) {
            d->m_canvas->addWidget(block.panel, block.titleBar);
        }

        switch (state) {
            case Normal:
                if (!foldBtn->isChecked()) {
                    foldBtn->blockSignals(true);
                    foldBtn->setChecked(true);
                    foldBtn->blockSignals(false);
                }
                block.panel->setVisible(true);
                break;
            case Folded:
                if (foldBtn->isChecked()) {
                    foldBtn->blockSignals(true);
                    foldBtn->setChecked(false);
                    foldBtn->blockSignals(false);
                }
                block.panel->setVisible(false);
                break;
            case Hidden:
                d->m_canvas->removeWidget(block.panel);
                break;
        }

        block.state = state;

        emit floatingPanelStateChanged(key, state);
    }

    QWidget *PianoRoll::floatingPanel(const QString &key) {
        Q_D(const PianoRoll);
        auto it = d->floatingPanels.find(key);
        if (it == d->floatingPanels.end()) {
            return nullptr;
        }
        return it->panel;
    }

    PianoRoll::PianoRoll(PianoRollPrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
