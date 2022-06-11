#include "QUtauStd.h"
#include "QControlPoint.h"
#include "QGenonSettings.h"
#include "QLinkNote.h"
#include "QUtauStd_p.h"

#include <QFileInfo>

#define FRQ_SUFFIX QString("_wav.frq")

QString Utau::fromFrqName(const QString &filename) {
    if (filename.endsWith(FRQ_SUFFIX)) {
        return filename.mid(0, filename.size() - FRQ_SUFFIX.size()) + ".wav";
    }
    return "";
}

QString Utau::toFrqName(const QString &filename) {
    int index = filename.lastIndexOf('.');
    if (index < 0) {
        return filename + FRQ_SUFFIX;
    } else {
        return filename.chopped(index + 1) + FRQ_SUFFIX;
    }
}

QString Utau::mrqName() {
    return "desc.mrq";
}

QList<QControlPoint> Utau::DefaultEnvelope() {
    return {{0, 0}, {5, 100}, {35, 100}, {0, 0}};
}

QList<double> Utau::DefaultVibrato() {
    return {65, 180, 35, 20, 20, 0, 0, 0};
}

QList<QControlPoint> Utau::DefaultPortamento() {
    return {{-15, 0}, {15, 0}};
}

QString Utau::PointToString(Utau::PointType type) {
    QString res;
    switch (type) {
    case sJoin:
        res = "";
        break;
    case linearJoin:
        res = "s";
    case rJoin:
        res = "r";
        break;
    case jJoin:
        res = "j";
        break;
    }
    return res;
}

Utau::PointType Utau::StringToPoint(const QString &name) {
    PointType res;
    if (name == "s") {
        res = linearJoin;
    } else if (name == "r") {
        res = rJoin;
    } else if (name == "j") {
        res = jJoin;
    } else {
        res = sJoin;
    }
    return res;
}

QList<QControlPoint> Utau::StringToPortamento(const Utau::PBStrings &pbstr) {
    QControlPoint p;
    QList<QControlPoint> res;

    if (pbstr.PBS.isEmpty() || pbstr.PBW.isEmpty()) {
        return {};
    }

    res.clear();
    QStringList PBSXY = pbstr.PBS.split(SIMICOLON);

    if (!PBSXY.isEmpty()) {
        p.mX = PBSXY.front().toDouble();
        if (PBSXY.size() >= 2) {
            p.mY = PBSXY.at(1).toDouble();
        } else {
            p.mY = 0;
        }
    }
    res.push_back(p);

    QStringList PBWs = pbstr.PBW.split(COMMA);
    QStringList PBYs = pbstr.PBY.split(COMMA);
    QStringList PBMs = pbstr.PBM.split(COMMA);

    for (int i = 0; i < qMax(PBWs.size(), PBYs.size()); i++) {
        p = QControlPoint();
        if (PBWs.size() > i) {
            p.mX = PBWs.at(i).toDouble();
        }
        if (PBYs.size() > i) {
            p.mY = (PBYs.at(i).isEmpty()) ? 0 : PBYs.at(i).toDouble();
        }
        if (PBMs.size() > i) {
            p.mP = Utau::StringToPoint(PBMs.at(i));
        }
        p.mX += res.back().mX;
        res.push_back(p);
    }

    // Negative Correction
    for (int i = 1; i < res.size(); ++i) {
        QControlPoint &curPoint = res[i];
        QControlPoint &prevPoint = res[i - 1];

        if (curPoint.mX < prevPoint.mX) {
            curPoint.mX = prevPoint.mX;
        }
    }

    return res;
}

Utau::PBStrings Utau::PortamentoToString(const QList<QControlPoint> &points) {
    QStringList strs;
    PBStrings res;
    if (points.isEmpty()) {
        return res;
    }

    // PBS
    QControlPoint first = points.front();
    strs.push_back(NUM2STR(first.mX));
    if (first.mY != 0) {
        strs.push_back(NUM2STR(first.mY));
    }
    res.PBS = strs.join(SIMICOLON);

    // PBW
    strs.clear();
    for (int i = 1; i < points.size(); i++) {
        strs.push_back(NUM2STR(points.at(i).mX - points.at(i - 1).mX));
    }
    res.PBW = strs.join(COMMA);

    // PBY
    strs.clear();
    for (int i = 1; i < points.size(); i++) {
        strs.push_back(NUM2STR(points.at(i).mY));
    }
    res.PBY = strs.join(COMMA);

    // PBM
    strs.clear();
    for (int i = 1; i < points.size(); i++) {
        strs.push_back(Utau::PointToString(points.at(i).mP));
    }
    res.PBM = strs.join(COMMA);
    return res;
}

QList<QControlPoint> Utau::StringToEnvelope(const QString &str) {
    QStringList strList = str.split(COMMA);
    QList<double> nums;
    QList<QControlPoint> res;

    if (strList.size() >= 8) {
        strList.removeAt(7); // Remove %
    } else if (strList.size() < 7) {
        return {}; // Invalid
    }
    for (auto it = strList.begin(); it != strList.end(); ++it) {
        nums.append(it->toDouble());
    }
    if (nums.size() % 2 != 0) {
        nums.push_back(0);
    }
    res.push_back(QControlPoint(nums.at(0), nums.at(3)));
    res.push_back(QControlPoint(nums.at(1), nums.at(4)));
    if (nums.size() == 10) {
        res.push_back(QControlPoint(nums.at(8), nums.at(9)));
    }
    res.push_back(QControlPoint(nums.at(2), nums.at(5)));
    res.push_back(QControlPoint(nums.at(7), nums.at(6)));

    return res;
}

QString Utau::EnvelopeToString(const QList<QControlPoint> &points) {
    int offset;
    QList<double> nums;
    QStringList res;

    if (points.size() < 4) {
        return "";
    }

    offset = (points.size() == 5);
    nums.push_back(points.at(0).mX);
    nums.push_back(points.at(1).mX);
    nums.push_back(points.at(2 + offset).mX);
    nums.push_back(points.at(0).mY);
    nums.push_back(points.at(1).mY);
    nums.push_back(points.at(2 + offset).mY);
    nums.push_back(points.at(3 + offset).mY);
    nums.push_back(points.at(3 + offset).mX);

    if (points.size() == 5) {
        nums.push_back(points.at(2).mX);
        nums.push_back(points.at(2).mY);
    }
    if (nums.size() == 8 && nums.at(7) == 0.0) {
        nums.pop_back();
    }
    for (auto it = nums.begin(); it != nums.end(); ++it) {
        res.append(NUM2STR(*it));
    }
    if (res.size() > 7) {
        res.insert(7, "%");
    }
    return res.join(COMMA);
}

Utau::Genon Utau::StringToGenon(const QString &str) {
    if (str.isEmpty()) {
        return Genon();
    }

    QStringList settingsStrs = str.split(COMMA);
    QStringList namesStrs = settingsStrs.front().split(EQUAL);
    QString alias;

    if (namesStrs.size() >= 2) {
        alias = namesStrs.at(1);
    }

    QString fileName = namesStrs.front();
    while (settingsStrs.size() < 6) {
        settingsStrs.append("0");
    }

    QGenonSettings res;
    res.mFileName = fileName;
    res.mAlias = alias;
    res.mOffset = settingsStrs.at(1).toDouble();
    res.mConstant = settingsStrs.at(2).toDouble();
    res.mBlank = settingsStrs.at(3).toDouble();
    res.mPreUtterance = settingsStrs.at(4).toDouble();
    res.mVoiceOverlap = settingsStrs.at(5).toDouble();

    return res;
}

QString Utau::GenonToString(const Utau::Genon &genon) {
    QString res;
    QTextStream out(&res);

    out << QFileInfo(genon.mFileName).fileName() << EQUAL;
    out << genon.mAlias << COMMA;
    out << genon.mOffset << COMMA;
    out << genon.mConstant << COMMA;
    out << genon.mBlank << COMMA;
    out << genon.mPreUtterance << COMMA;
    out << genon.mVoiceOverlap;

    return res;
}

QList<double> Utau::StringsToDoubles(const QStringList &strs) {
    QList<double> nums;
    for (auto it = strs.begin(); it != strs.end(); ++it) {
        nums.append(it->isEmpty() ? 0.0 : it->toDouble());
    }
    return nums;
}

QStringList Utau::DoublesToStrings(const QList<double> &nums) {
    QStringList strs;
    for (auto it = nums.begin(); it != nums.end(); ++it) {
        strs.append(((*it) == 0) ? "" : NUM2STR(*it));
    }
    while (!strs.isEmpty() && strs.back().isEmpty()) {
        strs.pop_back();
    }
    return strs;
}

int Utau::ToneNameToToneNum(const QString &name) {
    int octave;

    if (name.length() < 2) {
        return TONE_NUMBER_BASE;
    }

    int index = QString(TONE_NAMES).indexOf(name.front());
    if (index < 0) {
        index = 0;
    }

    octave = name.rightRef(1).toInt();

    if (octave < TONE_OCTAVE_MIN) {
        octave = TONE_OCTAVE_MIN;
    } else if (octave > TONE_OCTAVE_MAX) {
        octave = TONE_OCTAVE_MAX;
    }

    return TONE_NUMBER_BASE + (octave - 1) * TONE_OCTAVE_STEPS + index +
           static_cast<int>(name[1] == TONE_NAME_SHARP);
}

QString Utau::ToneNumToToneName(int num) {
    return ToneNumToToneName(num % TONE_OCTAVE_STEPS, num / TONE_OCTAVE_STEPS - 2);
}

QString Utau::ToneNumToToneName(int nameIndex, int octaveIndex) {
    QString tone_names(TONE_NAMES);
    QString name = tone_names.at(nameIndex);
    if (nameIndex > 0 && tone_names.at(nameIndex) == tone_names.at(nameIndex - 1)) {
        name += TONE_NAME_SHARP;
    }
    name += NUM2STR(octaveIndex + 1);
    return name;
}

double Utau::TickToTime(int tick, double tempo) {
    return (static_cast<unsigned long>(60000) * tick) / (tempo * TIME_BASE);
}

int Utau::TimeToTick(double time, double tempo) {
    return static_cast<int>(time * tempo * TIME_BASE / 60000);
}

bool Utau::isRestLyric(const QString &lyric) {
    QString lrc = lyric.trimmed();
    return (lrc.isEmpty() || !lrc.compare(LYRIC_R, Qt::CaseInsensitive));
}

int Utau::StandardToneNum() {
    return (4 - TONE_OCTAVE_MIN) * TONE_OCTAVE_STEPS + TONE_NUMBER_BASE;
}
