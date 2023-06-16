#include "DspxBaseEntity.h"
#include "DspxEntityUtils_p.h"

namespace Core {

    static bool compareIntPointX(const AceTreeEntity *left, const AceTreeEntity *right) {
        return left->property("x").toInt() < right->property("x").toInt();
    }

    static bool compareDoublePointX(const AceTreeEntity *left, const AceTreeEntity *right) {
        return left->property("x").toDouble() < right->property("x").toDouble();
    }

    //===========================================================================
    // BusControl
    DspxBusControlEntity::DspxBusControlEntity(QObject *parent) : AceTreeEntityMapping(parent) {
    }
    DspxBusControlEntity::~DspxBusControlEntity() {
    }
    QString DspxBusControlEntity::name() const {
        return "control";
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
    }
    DspxIntPointEntity::~DspxIntPointEntity() {
    }
    QString DspxIntPointEntity::name() const {
        return "point";
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
    }
    DspxIntPointListEntity::~DspxIntPointListEntity() {
    }
    QString DspxIntPointListEntity::name() const {
        return "points";
    }
    void DspxIntPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareIntPointX);
    }
    //===========================================================================

    //===========================================================================
    // DoublePoint
    DspxDoublePointEntity::DspxDoublePointEntity(QObject *parent) : AceTreeEntityMapping(parent) {
    }
    DspxDoublePointEntity::~DspxDoublePointEntity() {
    }
    QString DspxDoublePointEntity::name() const {
        return "point";
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
    }
    DspxDoublePointListEntity::~DspxDoublePointListEntity() {
    }
    QString DspxDoublePointListEntity::name() const {
        return "points";
    }
    void DspxDoublePointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareDoublePointX);
    }
    //===========================================================================

    //===========================================================================
    // AnchorPoint
    DspxAnchorPointEntity::DspxAnchorPointEntity(QObject *parent) : DspxIntPointListEntity(parent) {
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
    }
    DspxAnchorPointListEntity::~DspxAnchorPointListEntity() {
    }
    QString DspxAnchorPointListEntity::name() const {
        return "points";
    }
    void DspxAnchorPointListEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
        std::sort(records.begin(), records.end(), compareIntPointX);
    }
    //===========================================================================

    //===========================================================================
    // Workspace
    DspxWorkspaceEntity::DspxWorkspaceEntity(QObject *parent) : AceTreeObjectEntity(DynamicData, parent) {
    }

    DspxWorkspaceEntity::~DspxWorkspaceEntity() {
    }

    QString DspxWorkspaceEntity::name() const {
        return "workspace";
    }
    //===========================================================================

}