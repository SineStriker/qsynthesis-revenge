#include "IDsPorter.h"

IDsPorter::IDsPorter(QObject *parent) : INamePlugin(parent) {
}

IDsPorter::~IDsPorter() {
}

IDsPorter::SupportedPorts IDsPorter::ports() const {
    return All;
}
