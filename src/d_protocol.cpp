#include "dprotocol/d_protocol.hpp"

DProtocal::DProtocal() {}


DProtocal::~DProtocal() {}


uint8_t DProtocal::retrieveMessage(CircularByteArray *cba, DProtocolMessage *message) {
  // first get 2 bytes from the cba to check if it's the d protocol message

  if (cba->size() <= 8) {
    // no enough data
    return 1;
  }
  
  if (cba->peek((uint8_t *)&message_header, 2, 0) == 0 && message_header == MESSAGE_START_MAGIC_NUMBER) {
    // check the protocol version
    if (cba->peek(&protocol_version, 1, 2) == 0 && protocol_version == 1) {
      cba->peek((uint8_t *)&messageLength, 4, 4);
      // then retrieve the message and translate to d protocol message
      message->protocolVersion = 1;
      message->clientId = 1;
      message->length = messageLength;

      // check if current message has already been transformed completely
      if (cba->size() == (10 + messageLength)
          || (cba->peek((uint8_t *)&message_header, 2, 10 + messageLength) == 0 && message_header == MESSAGE_START_MAGIC_NUMBER)) {
        cba->peek((uint8_t *)&message->type, 2, 8);
        message->body = new uint8_t[messageLength];
        cba->peek(message->body, messageLength, 10);
        cba->remove(messageLength + 10);
        return 0;
      } else if (cba->size() < (10 + messageLength)) {
        return 1;
      } else if (cba->peek((uint8_t *)&message_header, 2, 10 + messageLength) == 0 && message_header != MESSAGE_START_MAGIC_NUMBER) {
        // encounter illegal message head
        return 10;
      }
    } else {
      std::cout << "illegal protocol version. version: " << protocol_version << std::endl;
      return 10;
    }
  } else {
    // the head message inside the cba is invalid
    std::cout << "illegal starting magic number. number: " << message_header << std::endl;
    return 10;
  }
}

uint8_t DProtocal::wrapMessage(uint8_t *&messageBinary, uint16_t messageType, uint8_t *data, uint16_t size) {
  *((uint16_t *) messageBinary) = MESSAGE_START_MAGIC_NUMBER;
  *((uint8_t *) messageBinary + 2) = 1;
  *((uint8_t *) messageBinary + 3) = 1;
  *((uint32_t *) (messageBinary + 4)) = size;
  *((uint16_t *) (messageBinary + 8)) = messageType;
  memcpy(messageBinary + 10, data, size);
  return 0;
}


void DProtocal::cleanCBA(CircularByteArray *cba) {
  uint16_t head;
  uint16_t clean_num = 0;
  while (cba->size() > 0) {
    if (cba->peek((uint8_t *) &head, 2, 0) == 0) {
      if (head == MESSAGE_START_MAGIC_NUMBER) {
        return;
      } else {
        clean_num++;
        cba->remove(1);
      }
    } else { // size of the bytes inside the buffer is less than 2.
      clean_num++;
      cba->remove(1);
    }
  }
  std::cout << "Cleaning " << clean_num << std::endl;
}
