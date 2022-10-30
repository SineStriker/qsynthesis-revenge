#ifndef CHANNEL_H
#define CHANNEL_H

#include "labeler.h"
#include "plotorplayer.h"

class Channel {
public:
  Channel(QSet<Labeler *> labelers, QSet<PlotOrPlayer *> plotOrPlayers);

  ~Channel();

public:
  bool show();

private:
  QSet<Labeler *> labelers;
  QSet<PlotOrPlayer *> plotOrPlayers;
};

#endif // CHANNEL_H
