//
// Created by Crs_1 on 2023/4/5.
//

#ifndef CHORUSKIT_IPREFERENCEITEM_H
#define CHORUSKIT_IPREFERENCEITEM_H
#include <QObject>
namespace Core {
    class IPreferenceItem {
    public:
        enum NodeType {
            Page,
            Category,
        };
        virtual QString id() const = 0;
        virtual QString title() const = 0;
        virtual QString description() const = 0;
        virtual NodeType nodeType() = 0;
    };
}
#endif // CHORUSKIT_IPREFERENCEITEM_H
