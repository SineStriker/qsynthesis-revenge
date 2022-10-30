#ifndef PLOTORPLAYER_H
#define PLOTORPLAYER_H

#include "range.h"

class PlotOrPlayer {
public:
  PlotOrPlayer();

  virtual ~PlotOrPlayer() = 0;

public:
  virtual bool updateRanges(const QSet<Range *> added,
                            const QSet<Range *> deleted,
                            const QSet<Range *> updated, void *labeler) = 0;

  virtual bool changeRangeFocus(Range *focus) = 0;

  virtual bool refresh() = 0;

  virtual bool clear() = 0;

private:
  QMap<void *, QSet<Range *>> allLabelerRanges;
  Range *focus;
};

#endif // PLOTORPLAYER_H
