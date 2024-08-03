#ifndef D_PROTOCOL_MESSAGE_HPP
#define D_PROTOCOL_MESSAGE_HPP

#include "stdint.h"

#define MESSAGE_START_MAGIC_NUMBER 0xFCFD

struct DProtocolMessage{
  uint16_t messageStartMagicNumber = MESSAGE_START_MAGIC_NUMBER;
  uint8_t protocolVersion;
  uint16_t length;
  uint16_t type;
  uint8_t *body;
};



#endif /* D_PROTOCOL_MESSAGE_HPP */
