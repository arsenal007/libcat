#ifndef COMMUNICATION_PORT_H_
#define COMMUNICATION_PORT_H_

#include <stdint.h>

// Enum for communication port types
typedef enum {
    COMMUNICATION_PORT_USB,  // USB CDC communication port
    COMMUNICATION_PORT_UART  // UART communication port
} communication_port_type_t;

// Structure to describe a communication port
typedef struct {
    communication_port_type_t type;  // Port type (USB or UART)
    uint8_t port_id;                 // Port identifier (e.g., 0 for USB CDC 0)
} communication_port_t;

// Initialize a specific communication port
int communication_port_init(const communication_port_t* port);

// Write data to a communication port
int communication_port_write(const communication_port_t* port, const uint8_t* buf, uint32_t len);

// Read data from a communication port
int communication_port_read(const communication_port_t* port, uint8_t* buf, uint32_t len);

// Check if data is available to read from a communication port
int communication_port_available(const communication_port_t* port);

#endif // COMMUNICATION_PORT_H_
