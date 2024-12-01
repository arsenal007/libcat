#include <stdint.h>
#include <libcat.h>

static char cat_cmd_rx_buffer[ CAT_CMD_MAX_LENGTH ];  // Buffer for received commands
static uint8_t cat_cmd_rx_index = 0;                  // Index of the current character in the buffer

#if defined( TESTS )

char* get_cat_cmd_rx_buffer( void )
{
  return cat_cmd_rx_buffer;
}

uint8_t get_cat_cmd_rx_index( void )
{
  return cat_cmd_rx_index;
}

#endif

// Function to process the received command
static void cat_decode_received_cmd( void )
{
  // Example: Output the command for debugging (can be replaced with actual processing)
  // printf("Received CAT command: %s\n", cat_cmd_rx_buffer);
  // Add your command processing logic here
}

// Function to receive and assemble the command
void cat_receive_cmd( char cmd )
{
  // Add the character to the buffer
  cat_cmd_rx_buffer[ cat_cmd_rx_index ] = cmd;
  cat_cmd_rx_index++;

  // Check for buffer overflow
  if ( cat_cmd_rx_index >= CAT_CMD_MAX_LENGTH )
  {
    cat_cmd_rx_index = CAT_CMD_MAX_LENGTH - 1;  // Prevent overflow
  }

  // If the end character ';' is received
  if ( cmd == ';' )
  {
    cat_cmd_rx_buffer[ cat_cmd_rx_index ] = '\0';
    cat_decode_received_cmd();  // Process the command
    cat_cmd_rx_index = 0;       // Reset the index for the next command
  }
}
