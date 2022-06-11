#include "FileParser.h"
#include "QUtauSections.h"
#include "SystemHelper.h"

const char mid_suffix[] = "mid";
const char svp_suffix[] = "svp";
const char s5p_suffix[] = "s5p";
const char vsqx_suffix[] = "vsqx";
const char vsq_suffix[] = "vsq";
const char ustx_suffix[] = "ustx";

FileParser::FileParser(QWidget *parent) : QObject(parent) {
}

FileParser::~FileParser() {
}

bool FileParser::parseFile(const QString &filename, SectionNotes &notes) {
    QString suffix = Sys::PathFindSuffix(filename).toLower();
    if (suffix == mid_suffix) {
        SectionNotes temp;
        if (parseMidiFile(filename, temp)) {
            notes = temp;
            return true;
        }
    } else if (suffix == svp_suffix) {
        SectionNotes temp;
        if (parseSynthVFile(filename, temp)) {
            notes = temp;
            return true;
        }
    } else if (suffix == s5p_suffix) {
        SectionNotes temp;
        if (parseSynthVOldFile(filename, temp)) {
            notes = temp;
            return true;
        }
    } else if (suffix == vsqx_suffix) {
        SectionNotes temp;
        if (parseVocaloidFile(filename, temp)) {
            notes = temp;
            return true;
        }
    } else if (suffix == vsq_suffix) {
        SectionNotes temp;
        if (parseVocaloidOldFile(filename, temp)) {
            notes = temp;
            return true;
        }
    } else if (suffix == ustx_suffix) {
        SectionNotes temp;
        if (parseOpenUTAUFile(filename, temp)) {
            notes = temp;
            return true;
        }
    }
    return false;
}

bool FileParser::isSuffixParsable(const QString &suffix) {
    QStringList formats{mid_suffix, svp_suffix, s5p_suffix, vsqx_suffix, vsq_suffix, ustx_suffix};
    return formats.contains(suffix.toLower());
}
