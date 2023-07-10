//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_INAMEPROVIDER_H
#define CHORUSKIT_INAMEPROVIDER_H

#include <QString>

class INameProvider {
public:
    QString name() const;
protected:
    void setName(const QString &name);
private:
    QString m_name;
};



#endif // CHORUSKIT_INAMEPROVIDER_H
