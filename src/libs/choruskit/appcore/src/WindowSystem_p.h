#ifndef WINDOWSYSTEM_P_H
#define WINDOWSYSTEM_P_H

#include <QHash>
#include <QSet>

#include "QMChronMap.h"
#include "QMChronSet.h"

#include "WindowSystem.h"

namespace Core {

    struct WindowGeometry {
        QRect geometry;
        bool maximized;
        double ratio;

        WindowGeometry() : maximized(false), ratio(0){};
        WindowGeometry(const QRect &rect, bool max = false)
            : geometry(rect), maximized(max), ratio(0){};

        static WindowGeometry fromObject(const QJsonObject &obj);
        QJsonObject toObject() const;
    };

    struct SplitterSizes {
        QList<int> sizes;

        SplitterSizes(const QList<int> &sizes = {}) : sizes(sizes){};

        static SplitterSizes fromObject(const QJsonObject &obj);
        QJsonObject toObject() const;
    };

    class WindowSizeTrimmer;

    class WindowSystemPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(WindowSystem)
    public:
        WindowSystemPrivate();
        virtual ~WindowSystemPrivate();

        void init();

        void readSettings();
        void saveSettings() const;

        WindowSystem *q_ptr;

        QHash<QString, QMChronMap<const QMetaObject *, WindowSystem::AddOnFactory>> addOnFactories;

        QMChronSet<IWindow *> iWindows;
        QHash<QWidget *, IWindow *> windowMap;

        QHash<QString, WindowGeometry> winGeometries;
        QHash<QString, SplitterSizes> splitterSizes;

        void windowClosed(IWindow *iWin);
    };
} // namespace Core

#endif // WINDOWSYSTEM_P_H
