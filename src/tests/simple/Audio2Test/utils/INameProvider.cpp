//
// Created by Crs_1 on 2023/7/10.
//

#include "INameProvider.h"

QString INameProvider::name() const {
    return m_name;
}

void INameProvider::setName(const QString &name) {
    this->m_name = name;
}
