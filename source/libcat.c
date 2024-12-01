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

typedef struct
{
  const uint8_t* command;                      // Pointer to the command bytes
  uint8_t command_length;                      // Length of the command
  void ( *handler )( const uint8_t* params );  // Handler for the command
  uint8_t param_length;                        // Expected parameter length (0 if none required)
} Command;

// Example handlers
void Command_IF( const uint8_t* )
{
  // printf( "IF command executed\n" );
}
void Command_FA( const uint8_t* )
{
  // printf( "Frequency set: %.*s\n", 11, params );
}
void Command_MD( const uint8_t* )
{
  // printf( "MD command executed\n" );
}

// Command definitions
const uint8_t CMD_IF[] = { 'I', 'F', ';' };
const uint8_t CMD_FA[] = { 'F', 'A' };
const uint8_t CMD_MD[] = { 'M', 'D', ';' };

Command command_table[] = {
  { CMD_IF, 3, Command_IF, 0 },
  { CMD_FA, 2, Command_FA, 11 },
  { CMD_MD, 3, Command_MD, 0 },
};

#define COMMAND_TABLE_SIZE ( sizeof( command_table ) / sizeof( command_table[ 0 ] ) )

// Function to compare a command in the buffer with a given command template
// Parameters:
//   buffer: Pointer to the buffer containing the received command
//   command: Pointer to the command template to compare against
//   command_length: Length of the command to compare
// Returns:
//   1 if the buffer matches the command template, 0 otherwise
static uint8_t compare_command( const uint8_t* buffer, const uint8_t* command, uint8_t command_length )
{
  uint8_t i;
  // Iterate through each byte of the command
  for ( i = 0; i < command_length; i++ )
  {
    // If any byte does not match, return 0 (mismatch)
    if ( buffer[ i ] != command[ i ] )
    {
      return 0;
    }
  }
  // If all bytes match, return 1 (match)
  return 1;
}

static void copy( uint8_t* dst, const char* src, uint8_t size )
{
  for ( uint8_t i = 0; i < size; i++ )
    dst[ i ] = src[ i ];
}

static void set( uint8_t* dst, uint8_t value, uint8_t size )
{
  for ( uint8_t i = 0; i < size; i++ )
    dst[ i ] = value;
}

static void cat_decode_received_cmd( const char* cmd_buffer, uint8_t buffer_length )
{
  uint8_t buffer[ CAT_CMD_MAX_LENGTH ];
  set( buffer, 0u, CAT_CMD_MAX_LENGTH );
  copy( buffer, cmd_buffer, buffer_length );

  uint8_t i;

  for ( i = 0; i < COMMAND_TABLE_SIZE; i++ )
  {
    const uint8_t* cmd = (const uint8_t*)command_table[ i ].command;
    uint8_t cmd_length = command_table[ i ].command_length;

    // Compare the received command with the command in the table
    if ( compare_command( buffer, cmd, cmd_length ) == 1 )
    {
      // Calculate the length of the parameters
      uint8_t param_length = buffer_length - cmd_length;

      // Check if the parameter length matches the expected length
      if ( param_length == command_table[ i ].param_length || command_table[ i ].param_length == 0 )
      {
        // Call the handler with the parameters
        command_table[ i ].handler( buffer + cmd_length );
        return;
      }
      else
      {
        // Invalid parameter length, return without action
        return;
      }
    }
  }

  // If no matching command is found, you can handle it here (optional)
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
    cat_cmd_rx_buffer[ cat_cmd_rx_index ] = '\0';                    // needs for tests
    cat_decode_received_cmd( cat_cmd_rx_buffer, cat_cmd_rx_index );  // Process the command
    cat_cmd_rx_index = 0;                                            // Reset the index for the next command
  }
}
