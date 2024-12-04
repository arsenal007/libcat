#include <stdint.h>
#include <libcat.h>
#include <commands_table_t.h>

static char cat_cmd_rx_buffer[ CAT_CMD_MAX_LENGTH ];  // Buffer for received commands
static uint8_t cat_cmd_rx_index = 0;                  // Index of the current character in the buffer

static int cat_frequency_10GHz = 0;
static int cat_frequency_1GHz = 0;
static int cat_frequency_100MHz = 0;
static int cat_frequency_10MHz = 0;
static int cat_frequency_1MHz = 4;
static int cat_frequency_100kHz = 1;
static int cat_frequency_10kHz = 2;
static int cat_frequency_1kHz = 3;
static int cat_frequency_100Hz = 4;
static int cat_frequency_10Hz = 5;
static int cat_frequency_1Hz = 6;
static int8_t cat_RIT = 0;
static int8_t cat_XIT = 0;
static int8_t cat_MEM1 = 0;
static int8_t cat_MEM2 = 0;
static int8_t cat_RX = 0;
static int8_t cat_TX = 0;
static int8_t cat_VFO = 0;
static int8_t cat_SCAN = 0;
static int8_t cat_SIMPLEX = 0;
static int8_t cat_CTCSS = 0;
static int8_t cat_TONE1 = 0;
static int8_t cat_TONE2 = 0;
static int8_t cat_MODE = 2;

// Helper function to convert a frequency component to a character
static char convert_to_char( int value )
{
  return ( value + '0' );  // Convert integer (0-9) to corresponding ASCII character
}

static char cat_answer_rit_xit_13( int8_t rit, int8_t xit )
{
  // Check for RIT first
  if ( rit > 0 )
  {
    return '+';
  }
  else if ( rit < 0 )
  {
    return '-';
  }
  // Check for XIT if RIT is not active
  else if ( xit > 0 )
  {
    return '+';
  }
  else if ( xit < 0 )
  {
    return '-';
  }
  // Default case: no offset
  return ' ';
}

// Command handler for "IF;" command
static void Command_IF( const uint8_t* )
{
  // Define the response buffer with the correct size
  char cat_answer_buffer[ 21u ] = { 'I', 'F', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                                    '0', '0', '+', '0', '0', '0', '0', '0', '0', ';' };

  // Fill frequency components in the response buffer
  cat_answer_buffer[ 2 ] = convert_to_char( cat_frequency_10GHz );
  cat_answer_buffer[ 3 ] = convert_to_char( cat_frequency_1GHz );
  cat_answer_buffer[ 4 ] = convert_to_char( cat_frequency_100MHz );
  cat_answer_buffer[ 5 ] = convert_to_char( cat_frequency_10MHz );
  cat_answer_buffer[ 6 ] = convert_to_char( cat_frequency_1MHz );
  cat_answer_buffer[ 7 ] = convert_to_char( cat_frequency_100kHz );
  cat_answer_buffer[ 8 ] = convert_to_char( cat_frequency_10kHz );
  cat_answer_buffer[ 9 ] = convert_to_char( cat_frequency_1kHz );
  cat_answer_buffer[ 10 ] = convert_to_char( cat_frequency_100Hz );
  cat_answer_buffer[ 11 ] = convert_to_char( cat_frequency_10Hz );
  cat_answer_buffer[ 12 ] = convert_to_char( cat_frequency_1Hz );
  cat_answer_buffer[ 13 ] = ' ';
  cat_answer_buffer[ 14 ] = ' ';
  cat_answer_buffer[ 15 ] = ' ';
  cat_answer_buffer[ 16 ] = ' ';
  cat_answer_buffer[ 17 ] = ' ';
  cat_answer_rit_xit_13( cat_RIT, cat_XIT );
}

static void Command_FA( const uint8_t* )
{
  // printf( "Frequency set: %.*s\n", 11, params );
}

static void Command_MD( const uint8_t* )
{
  // printf( "MD command executed\n" );
}

// Command definitions
const uint8_t CMD_IF[] = { 'I', 'F', ';' };
const uint8_t CMD_FA[] = { 'F', 'A' };
const uint8_t CMD_MD[] = { 'M', 'D', ';' };

static commands_table_t command_table_release[] = {
  { CMD_IF, 3, Command_IF, 0 },
  { CMD_FA, 2, Command_FA, 11 },
  { CMD_MD, 3, Command_MD, 0 },
};

#define COMMAND_TABLE_SIZE 3

// Function to compare a command in the buffer with a given command template
// Parameters:
//   buffer: Pointer to the buffer containing the received command
//   command: Pointer to the command template to compare against
//   command_length: Length of the command to compare
// Returns:
//   1 if the buffer matches the command template, 0 otherwise
static uint8_t buffer_and_command_are_same( const uint8_t* buffer, const uint8_t* command, uint8_t command_length )
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

static void cat_decode_received_cmd( const char* cmd_buffer, uint8_t buffer_length,
                                     const commands_table_t* command_table, uint8_t command_table_size )
{
  uint8_t buffer[ CAT_CMD_MAX_LENGTH ];
  set( buffer, 0u, CAT_CMD_MAX_LENGTH );
  copy( buffer, cmd_buffer, buffer_length );

  for ( uint8_t i = 0; i < command_table_size; i++ )
  {
    const uint8_t* cmd = (const uint8_t*)command_table[ i ].command;
    uint8_t cmd_length = command_table[ i ].command_length;

    // Compare the received command with the command in the table
    if ( buffer_and_command_are_same( buffer, cmd, cmd_length ) == 1 )
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
    cat_cmd_rx_buffer[ cat_cmd_rx_index ] = '\0';  // needs for tests
    cat_decode_received_cmd( cat_cmd_rx_buffer, cat_cmd_rx_index, command_table_release,
                             COMMAND_TABLE_SIZE );  // Process the command
    cat_cmd_rx_index = 0;                           // Reset the index for the next command
  }
}

#if defined( TESTS )

char* get_cat_cmd_rx_buffer( void )
{
  return cat_cmd_rx_buffer;
}

uint8_t get_cat_cmd_rx_index( void )
{
  return cat_cmd_rx_index;
}

// Wrapper for buffer_and_command_are_same
uint8_t test_buffer_and_command_are_same( const uint8_t* buffer, const uint8_t* command, uint8_t command_length )
{
  return buffer_and_command_are_same( buffer, command, command_length );
}

// Wrapper for copy
void test_copy( uint8_t* dst, const char* src, uint8_t size )
{
  copy( dst, src, size );
}

// Wrapper for set
void test_set( uint8_t* dst, uint8_t value, uint8_t size )
{
  set( dst, value, size );
}

// Wrapper for cat_decode_received_cmd
void test_cat_decode_received_cmd( const char* cmd_buffer, uint8_t buffer_length, const commands_table_t* command_table,
                                   uint8_t command_table_size )
{
  cat_decode_received_cmd( cmd_buffer, buffer_length, command_table, command_table_size );
}

#endif