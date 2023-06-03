#include "PianoRoll.h"
#include "PianoRoll_p.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMetaEnum>

#include <CToolBar.h>
#include <CoreApi/ILoader.h>

#include "Internal/Widgets/PianoKeyWidget.h"

namespace Core {

    static const char settingCatalogC[] = "PianoRoll";

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

        m_pianoKeyContainer = new Internal::PianoKeyContainer();
        m_pianoKeyContainer->setObjectName("piano-container");

        m_sectionContainer = new QWidget();
        m_sectionContainer->setObjectName("section-container");

        m_layout = new QGridLayout();
        m_layout->setMargin(0);
        m_layout->setSpacing(0);
        m_layout->addItem(new QSpacerItem(0, 0), 0, 0);
        m_layout->addWidget(m_sectionContainer, 0, 1);
        m_layout->addWidget(m_pianoKeyContainer, 1, 0);
        m_layout->addWidget(m_canvas, 1, 1);

        q->setLayout(m_layout);
    }

    void PianoRollPrivate::readSettings() {
        Q_Q(PianoRoll);

        if (m_canvas->count() > 0) {
            for (const auto &item : qAsConst(floatingPanels)) {
                q->setFloatingPanelState(item.id, PianoRoll::Hidden);
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
            value = stateObj.value("id");
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
                {"id",    item.id                                                                    },
                {"state", QMetaEnum::fromType<PianoRoll::FloatingPanelState>().valueToKey(item.state)},
            });
        }
        obj.insert(floatingPanelsGroupC, floatingPanelsStateArr);

        // Piano key widget
        obj.insert(pianoKeyWidgetC, currentPianoKeyWidget);

        settings->insert(settingCatalogC, obj);
    }

    PianoRoll::PianoRoll(QWidget *parent) : PianoRoll(*new PianoRollPrivate(), parent) {
    }

    PianoRoll::~PianoRoll() {
    }

    void PianoRoll::readSettings() {
        Q_D(PianoRoll);
        d->readSettings();
    }

    void PianoRoll::saveSettings() const {
        Q_D(const PianoRoll);
        d->saveSettings();
    }

    void PianoRoll::addPianoKeyWidget(const QString &id, IPianoKeyWidgetFactory *factory) {
        Q_D(PianoRoll);
        if (!factory) {
            qWarning() << "Core::PianoRoll::addPianoKeyWidget(): trying to add null factory";
            return;
        }
        if (d->pianoKeyWidgets.contains(id)) {
            qWarning() << "Core::PianoRoll::addPianoKeyWidget(): trying to add duplicated factory:" << id;
            return;
        }
        d->pianoKeyWidgets.insert(id, factory);
    }

    QAbstractButton *PianoRoll::addFloatingPanel(const QString &id, QWidget *panel, QWidget *titleBar) {
        Q_D(PianoRoll);

        auto bar = new Internal::FloatingTitleBar();
        bar->setTitleBar(titleBar);

        connect(bar->foldButton(), &QAbstractButton::toggled, this, [this, id](bool checked) {
            setFloatingPanelState(id, checked ? Normal : Folded); //
        });

        connect(bar->closeButton(), &QAbstractButton::clicked, this, [id, this]() {
            setFloatingPanelState(id, Hidden); //
        });

        d->floatingPanels.insert(id, {id, panel, bar, Hidden});

        return bar->titleButton();
    }

    QStringList PianoRoll::pianoKeyWidgets() const {
        Q_D(const PianoRoll);
        return d->pianoKeyWidgets.keys();
    }


    QString PianoRoll::currentPianoKeyWidget() const {
        Q_D(const PianoRoll);
        return d->currentPianoKeyWidget;
    }

    void PianoRoll::setCurrentPianoKeyWidget(const QString &id) {
        Q_D(PianoRoll);

        if (d->currentPianoKeyWidget == id) {
            return;
        }

        auto fac = d->pianoKeyWidgets.value(id);
        if (!fac) {
            return;
        }
        d->currentPianoKeyWidget = id;

        // Remove old widget
        auto org = d->m_pianoKeyContainer->area();
        if (org) {
            org->deleteLater();
        }

        // Create new widget
        d->m_pianoKeyContainer->setArea(fac->create(nullptr));

        emit pianoKeyWidgetChanged(id);
    }

    IPianoKeyWidgetFactory *PianoRoll::pianoKeyWidgetFactory(const QString &id) const {
        Q_D(const PianoRoll);
        return d->pianoKeyWidgets.value(id);;
    }

    QStringList PianoRoll::floatingPanels() const {
        Q_D(const PianoRoll);
        return d->floatingPanels.keys();
    }

    PianoRoll::FloatingPanelState PianoRoll::floatingPanelState(const QString &id) {
        Q_D(const PianoRoll);
        auto it = d->floatingPanels.find(id);
        if (it == d->floatingPanels.end()) {
            return Hidden;
        }
        return it->state;
    }

    void PianoRoll::setFloatingPanelState(const QString &id, PianoRoll::FloatingPanelState state) {
        Q_D(PianoRoll);
        auto it = d->floatingPanels.find(id);
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

        emit floatingPanelStateChanged(id, state);
    }

    PianoRoll::PianoRoll(PianoRollPrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
