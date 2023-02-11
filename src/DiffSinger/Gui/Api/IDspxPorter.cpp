#include "IDspxPorter.h"

IDspxPorter::IDspxPorter(QObject *parent) : INamePlugin(parent) {
}

IDspxPorter::~IDspxPorter() {
}

IDspxPorter::SupportedPorts IDspxPorter::ports() const {
    return All;
}
