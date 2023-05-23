#ifndef CHORUSKIT_IDSPXOBSERVER_H
#define CHORUSKIT_IDSPXOBSERVER_H

#include <Dspx/QDspxModel.h>
#include <ItemModel/AceTreeModel.h>

namespace Core {

    class IDspxObserver {
    public:
        virtual bool read(const QDspx::Model &file, QsApi::AceTreeModel *mem, QString *errorMessage) = 0;
        virtual bool write(QDspx::Model *model, const QsApi::AceTreeModel *mem, QString *errorMessage) = 0;
    };

}



#endif // CHORUSKIT_IDSPXOBSERVER_H
