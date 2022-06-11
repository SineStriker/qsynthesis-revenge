#ifndef QUTAUBASIC_H
#define QUTAUBASIC_H

#include "Common/QControlPoint.h"
#include "Common/QCorrectGenon.h"
#include "Common/QGenonSettings.h"
#include "Common/QLinkNote.h"

double prop(double val, Utau::NoteProperty prop, double def = NODEF_DOUBLE);

QString prop(const QString &val, Utau::NoteProperty prop, const QString &def = NODEF_STRING);

#endif // QUTAUBASIC_H
