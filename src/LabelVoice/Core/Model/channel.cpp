#include "channel.h"
#include "labeler.h"
#include "plotorplayer.h"

Channel::Channel(QSet<Labeler *> labelers, QSet<PlotOrPlayer *> plotOrPlayers) {
  this->labelers = labelers;
  this->plotOrPlayers = plotOrPlayers;
}
