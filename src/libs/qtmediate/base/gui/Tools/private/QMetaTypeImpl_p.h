#ifndef QMETATYPEIMPLPRIVATE_H
#define QMETATYPEIMPLPRIVATE_H

#include <QString>

namespace QMetaTypeImpl {

    void Register();

    const char *TypeToFunctionName(int id);

    int FunctionNameToType(const QString &name);

}

#endif // QMETATYPEIMPLPRIVATE_H
