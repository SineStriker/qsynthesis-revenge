#include "HomeRecentWidget.h"

#include "ICore.h"
#include "IStyleHelper.h"

#include <QMDecoratorV2.h>

namespace Core {

    class HomeRecentTopButtonBar : public IButtonBar {
    public:
        HomeRecentTopFrame *frame;

        explicit HomeRecentTopButtonBar(HomeRecentTopFrame *frame) : frame(frame) {
            setParent(frame);
        }

        QAbstractButton *addButton(const QString &id) override {
            return frame->addButton(id);
        }

        void removeButton(const QString &id) override {
            frame->removeButton(id);
        }
    };

    /**
     * @brief Recent widget top frame
     */
    HomeRecentTopFrame::HomeRecentTopFrame(QWidget *parent) : QFrame(parent) {
        searchBox = new QLineEdit();
        searchBox->setClearButtonEnabled(true);
        searchBox->setObjectName("search-box");

        IStyleHelper::autoPolishPopupMenu(searchBox);

        newButton = new CTabButton();
        newButton->setProperty("type", "top-button");
        newButton->setObjectName("new-button");

        openButton = new CTabButton();
        openButton->setProperty("type", "top-button");
        openButton->setObjectName("open-button");

        topLayout = new QMEqualBoxLayout(QBoxLayout::LeftToRight);
        topLayout->setMargin(0);
        topLayout->setSpacing(0);

        topLayout->addWidget(searchBox);
        topLayout->addWidgetE(newButton);
        topLayout->addWidgetE(openButton);

        setLayout(topLayout);

        qIDec->installLocale(this, _LOC(HomeRecentTopFrame, this));

        connect(newButton, &QAbstractButton::clicked, this, &HomeRecentTopFrame::newRequested);
        connect(openButton, &QAbstractButton::clicked, this, &HomeRecentTopFrame::openRequested);

        // init interface
        m_buttonBar = new HomeRecentTopButtonBar(this);
    }

    HomeRecentTopFrame::~HomeRecentTopFrame() {
    }

    void HomeRecentTopFrame::reloadStrings() {
        newButton->setText(tr("New"));
        openButton->setText(tr("Open"));
        searchBox->setPlaceholderText(tr("Search for files"));
    }

    QAbstractButton *HomeRecentTopFrame::addButton(const QString &id) {
        if (externButtons.contains(id)) {
            return nullptr;
        }
        auto btn = new CTabButton();
        btn->setProperty("type", "top-button");
        topLayout->addWidgetE(btn);
        externButtons.insert(id, btn);
        return btn;
    }

    void HomeRecentTopFrame::removeButton(const QString &id) {
        auto btn = externButtons.value(id, nullptr);
        if (btn) {
            btn->deleteLater();
            externButtons.remove(id);
        }
    }

    IButtonBar *HomeRecentTopFrame::buttonBar() const {
        return m_buttonBar;
    }

    /**
     * @brief Recent widget
     */
    HomeRecentWidget::HomeRecentWidget(QWidget *parent) : QSplitter(Qt::Vertical, parent) {
        topWidget = new HomeRecentTopFrame();
        topWidget->setObjectName("top-widget");
        topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        bottomWidget = new LinearScrollArea(Qt::Vertical);
        bottomWidget->setObjectName("bottom-widget");
        bottomWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        setChildrenCollapsible(false);

        addWidget(topWidget);
        addWidget(bottomWidget);

        setStretchFactor(0, 0);
        setStretchFactor(1, 1);

        qIDec->installLocale(this, _LOC(HomeRecentWidget, this));

        connect(topWidget->searchBox, &QLineEdit::textChanged, this, &HomeRecentWidget::_q_searchTextChanged);
    }

    HomeRecentWidget::~HomeRecentWidget() {
    }

    void HomeRecentWidget::reloadStrings() {
    }

    void HomeRecentWidget::_q_searchTextChanged(const QString &text) {
        qDebug() << text;
    }

} // Core