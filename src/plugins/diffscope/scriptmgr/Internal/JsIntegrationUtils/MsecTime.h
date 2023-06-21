//
// Created by Crs_1 on 2023/6/2.
//

#ifndef CHORUSKIT_MSECTIME_H
#define CHORUSKIT_MSECTIME_H

#include <QString>
namespace ScriptMgr::Internal {

        class MsecTime {
            int t = 0;
        public:
            explicit MsecTime(int minute, int second, int msec);
            explicit MsecTime(int msec);
            explicit MsecTime(const QString &str);

            int minute() const;
            int second() const;
            int msec() const;
            int totalMsec() const;

            QString toString() const;
        };

    } // Internal

#endif // CHORUSKIT_MSECTIME_H
