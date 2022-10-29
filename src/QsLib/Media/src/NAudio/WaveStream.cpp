#include "WaveStream.h"

WaveStream::WaveStream() {
}

WaveStream::~WaveStream() {
}

void WaveStream::Flush() {
}

qint64 WaveStream::Seek(long offset, WaveStream::SeekOrigin origin) {
    switch (origin) {
        case SeekOrigin::Begin:
            SetPosition(offset);
            break;
        case SeekOrigin::Current:
            SetPosition(Position() + offset);
            break;
        default:
            SetPosition(Length() + offset);
            break;
    }
    return Position();
}

int WaveStream::Read(float *buffer, int offset, int count) {
    return Read((char *) buffer, offset * 4, count * 4);
}

int WaveStream::BlockAlign() const {
    return this->Format().BlockAlign();
}

void WaveStream::Skip(int seconds) {
    long num = this->Position() + (long) (this->Format().AverageBytesPerSecond() * seconds);
    if (num > this->Length())
        SetPosition(Length());
    else if (num < 0L)
        SetPosition(0);
    else
        SetPosition(num);
}

double WaveStream::CurrentTime() const {
    return (double) this->Position() / (double) this->Format().AverageBytesPerSecond();
}

void WaveStream::SetCurrentTime(double value) {
    SetPosition(value * (double) this->Format().AverageBytesPerSecond());
}

double WaveStream::TotalTime() const {
    return (double) this->Length() / (double) this->Format().AverageBytesPerSecond();
}

bool WaveStream::HasData(int count) const {
    Q_UNUSED(count);
    return this->Position() < this->Length();
}
