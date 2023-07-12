//
// Created by Crs_1 on 2023/7/10.
//

#include "IErrorStringProvider.h"

QString IErrorStringProvider::errorString() const {
    return m_error;
}
void IErrorStringProvider::setErrorString(const QString &error) {
    this->m_error = error;
}
void IErrorStringProvider::clearErrorString() {
    this->m_error.clear();
}
