//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_IERRORSTRINGPROVIDER_H
#define CHORUSKIT_IERRORSTRINGPROVIDER_H

#include <QString>

class IErrorStringProvider {
public:
    QString errorString() const;
protected:
    void setErrorString(const QString &error);
    void clearErrorString();
private:
    QString m_error;
};



#endif // CHORUSKIT_IERRORSTRINGPROVIDER_H
