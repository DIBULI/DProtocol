#ifndef D_PROTOCOL_HPP
#define D_PROTOCOL_HPP

#include <iostream>
#include "dprotocol/circular_byte_array.hpp"
#include "dprotocol/d_protocol_message.hpp"

class DProtocal {
public:
  DProtocal();
  ~DProtocal();

  // preserved space
  uint16_t message_header;
  uint8_t protocol_version;
  uint32_t messageLength;

  uint8_t retrieveMessage(CircularByteArray *cba, DProtocolMessage *message);

  uint8_t wrapMessage(uint8_t *&message, uint16_t messageType, uint8_t *data, uint16_t size);

  /**
   * @brief This method is used to clean up the illegal head message in the message receive buffer (CircularByteArray)
   * 
   * @param cba CircularByteArray to be cleaned
   */
  void cleanCBA(CircularByteArray *cba);
};


#endif /* D_PROTOCOL_HPP */
