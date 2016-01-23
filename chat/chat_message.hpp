#pragma once

#include <list>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_oarchive.hpp>

struct chat_message {
  void reset();

  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & type & message & login;
  }

  int type;
  std::string message;
  std::string login;

  enum {
    NEW_MSG = 0,
    PERSON_LEFT = 1,
    PERSON_CONNECTED = 2,
  };
};
