//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_IERRORPROVIDER_H
#define CHORUSKIT_IERRORPROVIDER_H

#include <QString>

class IErrorProvider {
public:
    QString error() const;
protected:
    void setError(const QString &error);
private:
    QString m_error;
};



#endif // CHORUSKIT_IERRORPROVIDER_H
