#include "IDspxPorter.h"

IDspxPorter::IDspxPorter(QObject *parent) : INamePlugin(parent) {
}

IDspxPorter::~IDspxPorter() {
}

QString IDspxPorter::categoryName() {
    return tr("Dspx format converter");
}

IDspxPorter::SupportedPorts IDspxPorter::ports() const {
    return All;
}
