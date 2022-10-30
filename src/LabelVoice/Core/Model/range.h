#ifndef RANGE_H
#define RANGE_H

#include <QList>
#include <QMap>
#include <QSet>

enum RANGE_NEIGHBOR_POSSIBLE {
  POSSIBLE_NEIGHBOR,
  INVALID_NEIGHBOR_POSITION_UNMATCH,  // the position of two ranges is not
                                      // correct
  INVALID_NEIGHBOR_MULTIPLE_CONFLICT, // one of the ranges already assigned to
                                      // another neighbor
};

enum RANGE_PARENT_POSSIBLE {
  POSSIBLE_PARENT,
  INVALID_PARENT_POSITION_UNMATCH,  // the position of two ranges is not correct
  INVALID_PARENT_MULTIPLE_CONFLICT, // the child range already assigned to
                                    // another parent
};

class Range {
public:
  Range(float begin, float end, int type, int hierarchy = -1);

  virtual ~Range();

public:
  bool setLeftNeighbor(Range *neighbor);

  bool setRightNeighbor(Range *neighbor);

  bool setParaent(Range *parent);

  static RANGE_NEIGHBOR_POSSIBLE neighborPossible(Range *left,
                                                       Range *right);

  static RANGE_PARENT_POSSIBLE parentPossible(Range *parent, Range *child);

  static Range *merge(QList<Range *> ranges);

  QList<Range *> split(QList<float> innerBounds);

  float begin, end;

  int type, hierarchy;

private:
  Range *leftNeighbor, *rightNeighbor, *parent;
};

#endif // RANGE_H
