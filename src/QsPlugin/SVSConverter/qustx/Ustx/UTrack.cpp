#include "UTrack.h"

UTrack::UTrack() {
    clear();
}

UTrack::~UTrack() {
}

void UTrack::clear() {
    singer.clear();
    phonemizer.clear();
    renderer.clear();
    mute = false;
    solo = false;
    volume = 1;
}
