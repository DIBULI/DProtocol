#include "dprotocol/d_protocol.hpp"

DProtocal::DProtocal() {}


DProtocal::~DProtocal() {}


uint8_t DProtocal::retrieveMessage(CircularByteArray *cba, DProtocolMessage *message) {
  // first get 2 bytes from the cba to check if it's the d protocol message
  
  if (cba->peek((uint8_t *)&message_header, 2, 0) && message_header == 0xFCFD) {
    // check the protocol version
    if (cba->peek(&protocol_version, 1, 2) && protocol_version == 1) {
      cba->peek((uint8_t *)&messageLength, 1, 3);

      // then retrieve the message and translate to d protocol message
      message->protocolVersion = 1;
      message->length = messageLength;
      cba->peek((uint8_t *)&message->type, 2, 5);
      message->body = new uint8_t[messageLength - 2];
      cba->peek(message->body, messageLength - 2, 7);

      cba->remove(messageLength);
      return 0;
    }
  } else {
    // the head message inside the cba is invalid
    return 10;
  }
}

void DProtocal::cleanCBA(CircularByteArray *cba) {
  uint16_t head;
  while (cba->size() > 0) {
    if (cba->peek((uint8_t *) &head, 2, 0) == 0) {
      if (head == MESSAGE_START_MAGIC_NUMBER) {
        return;
      } else {
        cba->remove(1);
      }
    } else { // size of the bytes inside the buffer is less than 2.
      cba->clean();
    }
  }
}
