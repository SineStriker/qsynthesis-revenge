#ifndef LABELER_H
#define LABELER_H

#include "range.h"
#include "labelinfo.h"
#include "plotorplayer.h"

class Labeler {
public:
  Labeler();

  virtual ~Labeler() = 0;

public:
  bool link(PlotOrPlayer *plotOrPlayer);

  bool unlink(PlotOrPlayer *plotOrPlayer);

  QList<Range *> getRanges(int type = -1, int hierarchy = -1) const;

  LabelInfo getLabelInfo(Range *range) const;

  virtual bool setRangeFocus(Range *focus) = 0;

private:
  QSet<PlotOrPlayer *> linkedPlotOrPlayers;
  QList<Range *> ranges;
  QMap<Range *, LabelInfo *> labelInfos;
  Range *focus;
};

#endif // LABELER_H
