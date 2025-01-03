// FollowerMode.h
#ifndef FOLLOWER_MODE_H
#define FOLLOWER_MODE_H

#include <string>
#include <ostream>

enum class FollowerMode {
   POINT,
   LINE,
   CHORD,
   CHORD3,
   TRIANGLE
};

static std::string toString(FollowerMode mode);
FollowerMode toFollowerMode(const std::string& s);
std::ostream& operator<<(std::ostream& os, const FollowerMode& mode);

#endif // FOLLOWER_MODE_H