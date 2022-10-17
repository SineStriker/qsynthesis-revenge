#include "QUstUtils.h"

#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>

#include "Config/UtaConstants.h"
#include "Config/UtaFilenames.h"

#define FRQ_SUFFIX QString("_wav.frq")

#define NUM2STR(str) QString::number(str)

static const char COMMA = ',';
static const char SIMICOLON = ';';
static const char EQUAL = '=';
static const char SLASH = '/';

using namespace Utau;

QString QUstUtils::fromFrqName(const QString &filename) {
    if (filename.endsWith(FRQ_SUFFIX)) {
        return filename.mid(0, filename.size() - FRQ_SUFFIX.size()) + ".wav";
    }
    return "";
}

QString QUstUtils::toFrqName(const QString &filename) {
    int index = filename.lastIndexOf('.');
    if (index < 0) {
        return filename + FRQ_SUFFIX;
    } else {
        return filename.chopped(index + 1) + FRQ_SUFFIX;
    }
}

QString QUstUtils::mrqName() {
    return "desc.mrq";
}

QList<QUstPoint> QUstUtils::DefaultEnvelope() {
    return {{0, 0}, {5, 100}, {35, 100}, {0, 0}};
}

QList<double> QUstUtils::DefaultVibrato() {
    return {65, 180, 35, 20, 20, 0, 0, 0};
}

QList<QUstPoint> QUstUtils::DefaultPortamento() {
    return {{-15, 0}, {15, 0}};
}

QString QUstUtils::PointToString(QUstPoint::PointType type) {
    QString res;
    switch (type) {
        case QUstPoint::sJoin:
            res = "";
            break;
        case QUstPoint::linearJoin:
            res = "s";
        case QUstPoint::rJoin:
            res = "r";
            break;
        case QUstPoint::jJoin:
            res = "j";
            break;
    }
    return res;
}

QUstPoint::PointType QUstUtils::StringTpoint(const QString &name) {
    QUstPoint::PointType res;
    if (name == "s") {
        res = QUstPoint::linearJoin;
    } else if (name == "r") {
        res = QUstPoint::rJoin;
    } else if (name == "j") {
        res = QUstPoint::jJoin;
    } else {
        res = QUstPoint::sJoin;
    }
    return res;
}

QList<QUstPoint> QUstUtils::StringToPortamento(const QUstUtils::PBStrings &pbstr) {
    QUstPoint p;
    QList<QUstPoint> res;

    if (pbstr.PBS.isEmpty() || pbstr.PBW.isEmpty()) {
        return {};
    }

    res.clear();
    QStringList PBSXY = pbstr.PBS.split(SIMICOLON);

    if (!PBSXY.isEmpty()) {
        p.X = PBSXY.front().toDouble();
        if (PBSXY.size() >= 2) {
            p.Y = PBSXY.at(1).toDouble();
        } else {
            p.Y = 0;
        }
    }
    res.push_back(p);

    QStringList PBWs = pbstr.PBW.split(COMMA);
    QStringList PBYs = pbstr.PBY.split(COMMA);
    QStringList PBMs = pbstr.PBM.split(COMMA);

    for (int i = 0; i < qMax(PBWs.size(), PBYs.size()); i++) {
        p = QUstPoint();
        if (PBWs.size() > i) {
            p.X = PBWs.at(i).toDouble();
        }
        if (PBYs.size() > i) {
            p.Y = (PBYs.at(i).isEmpty()) ? 0 : PBYs.at(i).toDouble();
        }
        if (PBMs.size() > i) {
            p.P = QUstUtils::StringTpoint(PBMs.at(i));
        }
        p.X += res.back().X;
        res.push_back(p);
    }

    // Negative Correction
    for (int i = 1; i < res.size(); ++i) {
        QUstPoint &curPoint = res[i];
        QUstPoint &prevPoint = res[i - 1];

        if (curPoint.X < prevPoint.X) {
            curPoint.X = prevPoint.X;
        }
    }

    return res;
}

QUstUtils::PBStrings QUstUtils::PortamentoToString(const QList<QUstPoint> &points) {
    QStringList strs;
    PBStrings res;
    if (points.isEmpty()) {
        return res;
    }

    // PBS
    QUstPoint first = points.front();
    strs.push_back(NUM2STR(first.X));
    if (first.Y != 0) {
        strs.push_back(NUM2STR(first.Y));
    }
    res.PBS = strs.join(SIMICOLON);

    // PBW
    strs.clear();
    for (int i = 1; i < points.size(); i++) {
        strs.push_back(NUM2STR(points.at(i).X - points.at(i - 1).X));
    }
    res.PBW = strs.join(COMMA);

    // PBY
    strs.clear();
    for (int i = 1; i < points.size(); i++) {
        strs.push_back(NUM2STR(points.at(i).Y));
    }
    res.PBY = strs.join(COMMA);

    // PBM
    strs.clear();
    for (int i = 1; i < points.size(); i++) {
        strs.push_back(QUstUtils::PointToString(points.at(i).P));
    }
    res.PBM = strs.join(COMMA);
    return res;
}

QList<QUstPoint> QUstUtils::StringToEnvelope(const QString &str) {
    QStringList strList = str.split(COMMA);
    QList<double> nums;
    QList<QUstPoint> res;

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
    res.push_back(QUstPoint(nums.at(0), nums.at(3)));
    res.push_back(QUstPoint(nums.at(1), nums.at(4)));
    if (nums.size() == 10) {
        res.push_back(QUstPoint(nums.at(8), nums.at(9)));
    }
    res.push_back(QUstPoint(nums.at(2), nums.at(5)));
    res.push_back(QUstPoint(nums.at(7), nums.at(6)));

    return res;
}

QString QUstUtils::EnvelopeToString(const QList<QUstPoint> &points) {
    int offset;
    QList<double> nums;
    QStringList res;

    if (points.size() < 4) {
        return "";
    }

    offset = (points.size() == 5);
    nums.push_back(points.at(0).X);
    nums.push_back(points.at(1).X);
    nums.push_back(points.at(2 + offset).X);
    nums.push_back(points.at(0).Y);
    nums.push_back(points.at(1).Y);
    nums.push_back(points.at(2 + offset).Y);
    nums.push_back(points.at(3 + offset).Y);
    nums.push_back(points.at(3 + offset).X);

    if (points.size() == 5) {
        nums.push_back(points.at(2).X);
        nums.push_back(points.at(2).Y);
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

QOtoItem QUstUtils::StringToGenon(const QString &str) {
    if (str.isEmpty()) {
        return QOtoItem();
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

    QOtoItem res;
    res.FileName = fileName;
    res.Alias = alias;
    res.Offset = settingsStrs.at(1).toDouble();
    res.Constant = settingsStrs.at(2).toDouble();
    res.Blank = settingsStrs.at(3).toDouble();
    res.PreUtterance = settingsStrs.at(4).toDouble();
    res.VoiceOverlap = settingsStrs.at(5).toDouble();

    return res;
}

QString QUstUtils::GenonToString(const QOtoItem &genon) {
    QString res;
    QTextStream out(&res);

    out << QFileInfo(genon.FileName).fileName() << EQUAL;
    out << genon.Alias << COMMA;
    out << genon.Offset << COMMA;
    out << genon.Constant << COMMA;
    out << genon.Blank << COMMA;
    out << genon.PreUtterance << COMMA;
    out << genon.VoiceOverlap;

    return res;
}

QList<double> QUstUtils::StringsToDoubles(const QStringList &strs) {
    QList<double> nums;
    for (auto it = strs.begin(); it != strs.end(); ++it) {
        nums.append(it->isEmpty() ? 0.0 : it->toDouble());
    }
    return nums;
}

QStringList QUstUtils::DoublesToStrings(const QList<double> &nums) {
    QStringList strs;
    for (auto it = nums.begin(); it != nums.end(); ++it) {
        strs.append(((*it) == 0) ? "" : NUM2STR(*it));
    }
    while (!strs.isEmpty() && strs.back().isEmpty()) {
        strs.pop_back();
    }
    return strs;
}

int QUstUtils::ToneNameToToneNum(const QString &name) {
    int octave;

    if (name.length() < 2) {
        return Utau::TONE_NUMBER_BASE;
    }

    int index = QString(Utau::TONE_NAMES).indexOf(name.front());
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

QString QUstUtils::ToneNumToToneName(int num) {
    return ToneNumToToneName(num % TONE_OCTAVE_STEPS, num / TONE_OCTAVE_STEPS - 2);
}

QString QUstUtils::ToneNumToToneName(int nameIndex, int octaveIndex) {
    QString tone_names(TONE_NAMES);
    QString name = tone_names.at(nameIndex);
    if (nameIndex > 0 && tone_names.at(nameIndex) == tone_names.at(nameIndex - 1)) {
        name += TONE_NAME_SHARP;
    }
    name += NUM2STR(octaveIndex + 1);
    return name;
}

double QUstUtils::TickToTime(int tick, double tempo) {
    return (static_cast<unsigned long>(60000) * tick) / (tempo * TIME_BASE);
}

int QUstUtils::TimeToTick(double time, double tempo) {
    return static_cast<int>(time * tempo * TIME_BASE / 60000);
}

bool QUstUtils::isRestLyric(const QString &lyric) {
    QString lrc = lyric.trimmed();
    return (lrc.isEmpty() || !lrc.compare(LYRIC_R, Qt::CaseInsensitive));
}

int QUstUtils::StandardToneNum() {
    return (4 - TONE_OCTAVE_MIN) * TONE_OCTAVE_STEPS + TONE_NUMBER_BASE;
}

QString QUstUtils::fromUSTVoiceDir(const QString &filePath, const QString &voicePath) {
    QString path = QDir::fromNativeSeparators(filePath);
    QString voice = voicePath + SLASH;

    if (path.startsWith(DIR_VOICE_BASE)) {
        path = voice + path.mid(int(strlen(DIR_VOICE_BASE)));
    } else if (QFileInfo(path).isRelative()) {
        path = qApp->applicationDirPath() + SLASH + path;
    }

    return path;
}

QString QUstUtils::toUSTVoiceDir(const QString &filePath, const QString &voicePath) {
    QString path = filePath;
    QString voice = voicePath + SLASH;

    if (path.startsWith(voice)) {
        path = DIR_VOICE_BASE + path.mid(voice.size());
    }

    //    path = QDir::toNativeSeparators(path);
    return path;
}

QString QUstUtils::fromUSTToolsDir(const QString &filePath) {
    QString path = QDir::fromNativeSeparators(filePath);
    if (path.startsWith("./")) {
        path.remove(0, 2);
    }
    if (QDir::isRelativePath(path)) {
        path = qApp->applicationDirPath() + SLASH + path;
    }
    return path;
}

QString QUstUtils::toUSTToolsDir(const QString &filePath) {
    QString path = filePath;
    QString app = qApp->applicationDirPath() + SLASH;
    if (filePath.startsWith(app)) {
        path = path.mid(app.size());
    }
    //    path = QDir::toNativeSeparators(path);
    return path;
}

double QUstUtils::prop(double val, NoteProperty prop, double def) {
    double res = NODEF_DOUBLE;
    switch (prop) {
    case Intensity:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_INTENSITY : def) : val;
        break;
    case Modulation:
        res =
            (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_MODULATION : def) : val;
        break;
    case Velocity:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_VELOCITY : def) : val;
        break;
    case PreUtterance:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_PRE_UTTERANCE : def)
                                    : val;
        break;
    case VoiceOverlap:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_VOICE_OVERLAP : def)
                                    : val;
        break;
    case StartPoint:
        res =
            (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_START_POINT : def) : val;
        break;
    case Tempo:
        res = (val == NODEF_DOUBLE) ? ((def == NODEF_DOUBLE) ? DEFAULT_VALUE_TEMPO : def) : val;
        break;
    default:
        break;
    }
    return res;
}

QString QUstUtils::prop(const QString &val, NoteProperty prop, const QString &def) {
    QString res = NODEF_STRING;
    switch (prop) {
    case Flags:
        res = (val == NODEF_STRING) ? ((def == NODEF_STRING) ? DEFAULT_VALUE_FLAGS : def) : val;
        break;
    default:
        break;
    }
    return res;
}
