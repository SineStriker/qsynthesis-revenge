#ifndef LABELINFO_H
#define LABELINFO_H

#include <QString>

class LabelInfo {
public:
  LabelInfo(QString label);

  virtual ~LabelInfo();

public:
  QString label;
};

#endif // LABELINFO_H
