#include "range.h"

Range::Range(float begin, float end, int type, int hierarchy) {
  this->begin = begin;
  this->end = end;
  this->type = type;
  this->hierarchy = hierarchy;
}
