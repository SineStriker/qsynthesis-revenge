//
// Created by Crs_1 on 2023/7/10.
//

#include "IErrorProvider.h"

QString IErrorProvider::error() const {
    return m_error;
}
void IErrorProvider::setError(const QString &error) {
    this->m_error = error;
}
