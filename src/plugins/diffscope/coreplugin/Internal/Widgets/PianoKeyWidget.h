#ifndef CHORUSKIT_PIANOKEYWIDGET_H
#define CHORUSKIT_PIANOKEYWIDGET_H

#include <QLabel>
#include <QWidget>

#include <Widgets/CToolButton.h>

#include "PianoRoll/IPianoKeyWidget.h"
#include "QPixelSize.h"

namespace Core::Internal {

    extern const char DefaultPianoKeyWidget[];

    class PianoKey : public CToolButton {
        Q_OBJECT
    public:
        enum KeyColor {
            Black,
            White,
        };

        explicit PianoKey(KeyColor color, QWidget *parent = nullptr);
        ~PianoKey();

        KeyColor color() const;
        void setColor(KeyColor color);

        int posFactor() const;
        void setPosFactor(int posFactor);

        int heightFactor() const;
        void setHeightFactor(int heightFactor);

    private:
        KeyColor m_color;

        int m_posFactor;
        int m_heightFactor;
    };

    class PianoKeyWidgetFactory : public Core::IPianoKeyWidgetFactory {
    public:
        Core::IPianoKeyWidget *create(QWidget *parent) override;
        QString name() const override;
    };

    class PianoKeyWidget : public Core::IPianoKeyWidget {
        Q_OBJECT
        Q_PROPERTY(QPixelSize preferredWidth READ preferredWidth WRITE setPreferredWidth)
    public:
        explicit PianoKeyWidget(QWidget *parent = nullptr);
        explicit PianoKeyWidget(int currentHeight, QWidget *parent = nullptr);
        ~PianoKeyWidget();

        QPixelSize preferredWidth() const;
        void setPreferredWidth(const QPixelSize &width);

    private:
        void init();

    public:
        int currentHeight() const override;
        void setCurrentHeight(int currentHeight) override;

    protected:
        QList<PianoKey *> m_whites;
        QList<PianoKey *> m_blacks;
        QList<QLabel *> m_labels;

        QLabel *m_toneLabel;
        int m_currentNoteNum;
        int m_currentHeight;

        void adjustHeight();
        void handleUpdateToneLabel();

    private:
        void timerEvent(QTimerEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    signals:
    };

}


#endif // CHORUSKIT_PIANOKEYWIDGET_H
