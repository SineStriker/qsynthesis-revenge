#include "DspxBaseEntity.h"
#include "DspxEntityUtils_p.h"
#include "AceTreeStandardEntity_p.h"

namespace Core {

    //===========================================================================
    // BusControl
    DspxBusControlEntity::DspxBusControlEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        AceTreeStandardEntityPrivate::setName(this, "control");
    }
    DspxBusControlEntity::~DspxBusControlEntity() {
    }
    double DspxBusControlEntity::gain() const {
        return property("gain").toDouble();
    }
    void DspxBusControlEntity::setGain(double gain) {
        setProperty("gain", gain);
    }
    bool DspxBusControlEntity::mute() const {
        return property("mute").toBool();
    }
    void DspxBusControlEntity::setMute(bool mute) {
        setProperty("mute", mute);
    }
    //===========================================================================

    //===========================================================================
    // TrackControl
    DspxTrackControlEntity::DspxTrackControlEntity(QObject *parent) : DspxBusControlEntity(parent) {
    }
    DspxTrackControlEntity::~DspxTrackControlEntity() {
    }
    double DspxTrackControlEntity::pan() const {
        return property("pan").toDouble();
    }
    void DspxTrackControlEntity::setPan(double pan) {
        setProperty("pan", pan);
    }
    bool DspxTrackControlEntity::solo() const {
        return property("solo\"").toBool();
    }
    void DspxTrackControlEntity::setSolo(bool solo) {
        setProperty("solo", solo);
    }
    // ==========================================================================

    //===========================================================================
    // IntPoint
    DspxIntPointEntity::DspxIntPointEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        AceTreeStandardEntityPrivate::setName(this, "point");
    }
    DspxIntPointEntity::~DspxIntPointEntity() {
    }
    int DspxIntPointEntity::x() const {
        return property("x").toInt();
    }
    void DspxIntPointEntity::setX(int x) {
        setProperty("x", x);
    }
    int DspxIntPointEntity::y() const {
        return property("y").toInt();
    }
    void DspxIntPointEntity::setY(int y) {
        setProperty("y", y);
    }
    //===========================================================================

    //===========================================================================
    // IntPoint List
    DspxIntPointListEntity::DspxIntPointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "points");
    }
    DspxIntPointListEntity::~DspxIntPointListEntity() {
    }
    void DspxIntPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementX<int>);
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint
    DspxDoublePointEntity::DspxDoublePointEntity(QObject *parent) : AceTreeEntityMapping(parent) {
        AceTreeStandardEntityPrivate::setName(this, "point");
    }
    DspxDoublePointEntity::~DspxDoublePointEntity() {
    }
    double DspxDoublePointEntity::x() const {
        return property("x").toDouble();
    }
    void DspxDoublePointEntity::setX(double x) {
        setProperty("x", x);
    }
    double DspxDoublePointEntity::y() const {
        return property("y").toDouble();
    }
    void DspxDoublePointEntity::setY(double y) {
        setProperty("y", y);
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint List
    DspxDoublePointListEntity::DspxDoublePointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "points");
    }
    DspxDoublePointListEntity::~DspxDoublePointListEntity() {
    }
    void DspxDoublePointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementX<double>);
    }
    //===========================================================================

    //===========================================================================
    // AnchorPoint
    DspxAnchorPointEntity::DspxAnchorPointEntity(QObject *parent) : DspxIntPointEntity(parent) {
    }
    DspxAnchorPointEntity::~DspxAnchorPointEntity() {
    }
    DspxAnchorPointEntity::Interpolation DspxAnchorPointEntity::interpolation() const {
        return variantToEnum<Interpolation>(property("interp"));
    }
    void DspxAnchorPointEntity::setInterpolation(DspxAnchorPointEntity::Interpolation interpolation) {
        setProperty("interp", enumToString(interpolation));
    }
    //===========================================================================

    //===========================================================================
    // AnchorPoint List
    DspxAnchorPointListEntity::DspxAnchorPointListEntity(QObject *parent) : AceTreeEntityRecordTable(parent) {
        AceTreeStandardEntityPrivate::setName(this, "points");
    }
    DspxAnchorPointListEntity::~DspxAnchorPointListEntity() {
    }
    void DspxAnchorPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareElementX<int>);
    }
    //===========================================================================

}