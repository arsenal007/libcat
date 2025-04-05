#include <stdint.h>
#include <libcat.h>
#include <commands_table_t.h>
#include <tests/tests.h>

#define CAT_CMD_MAX_LENGTH 20u  // Maximum length of the command buffer

static char cat_cmd_rx_buffer[ CAT_CMD_MAX_LENGTH ];  // Buffer for received commands
static uint8_t cat_cmd_rx_index = 0;                  // Index of the current character in the buffer

static int cat_frequency_10GHz = 0;
static int cat_frequency_1GHz = 0;
static int cat_frequency_100MHz = 0;
static int cat_frequency_10MHz = 1;
static int cat_frequency_1MHz = 4;
static int cat_frequency_100kHz = 0;
static int cat_frequency_10kHz = 7;
static int cat_frequency_1kHz = 4;
static int cat_frequency_100Hz = 0;
static int cat_frequency_10Hz = 0;
static int cat_frequency_1Hz = 0;
static int8_t cat_RIT = 0;
static int8_t cat_XIT = 0;
// static int8_t cat_MEM1 = 0;
// static int8_t cat_MEM2 = 0;
// static int8_t cat_RX = 0;
// static int8_t cat_TX = 0;
// static int8_t cat_VFO = 0;
// static int8_t cat_SCAN = 0;
// static int8_t cat_SIMPLEX = 0;
// static int8_t cat_CTCSS = 0;
// static int8_t cat_TONE1 = 0;
// static int8_t cat_TONE2 = 0;

static uint8_t cat_MODE = 2;  // 1: LSB, 2: USB, 3: CW, 4: FM, 5: AM, 6: FSK, 7: CR-R, 8: Reserved, 9: FSK-R

static int cat_rit_frequency_10kHz = 0;
static int cat_rit_frequency_1kHz = 0;
static int cat_rit_frequency_100Hz = 0;
static int cat_rit_frequency_10Hz = 0;
static int cat_rit_frequency_1Hz = 0;

static int cat_xit_frequency_10kHz = 0;
static int cat_xit_frequency_1kHz = 0;
static int cat_xit_frequency_100Hz = 0;
static int cat_xit_frequency_10Hz = 0;
static int cat_xit_frequency_1Hz = 0;

static int16_t cat_rit_frequency = 0;
static int16_t cat_xit_frequency = 0;

static int cat_step_frequency_1kHz = 0;
static int cat_step_frequency_100Hz = 0;
static int cat_step_frequency_10Hz = 0;
static int cat_step_frequency_1Hz = 0;

// static uint16_t cat_step = 0;

static uint32_t vfo_a_frequency = 0;
// static uint32_t vfo_b_frequency = 0;

static CAT_T callbacks;

static uint32_t calculate_frequency( void )
{
  uint32_t frequency = 10000000000L * cat_frequency_10GHz;
  frequency += 1000000000L * cat_frequency_1GHz;
  frequency += 100000000L * cat_frequency_100MHz;
  frequency += 10000000L * cat_frequency_10MHz;
  frequency += 1000000L * cat_frequency_1MHz;
  frequency += 100000L * cat_frequency_100kHz;
  frequency += 10000L * cat_frequency_10kHz;
  frequency += 1000L * cat_frequency_1kHz;
  frequency += 100L * cat_frequency_100Hz;
  frequency += 10L * cat_frequency_10Hz;
  frequency += cat_frequency_1Hz;
  return ( frequency );
}

// Helper function to convert a frequency component to a character
static char convert_to_char( int value )
{
  return ( value + '0' );  // Convert integer (0-9) to corresponding ASCII character
}

static char cat_answer_rit_xit_18( int8_t rit, int8_t xit )
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
static void handle_IF_query( const uint8_t* )
{
  // Define the response buffer with the correct size
  char cat_answer_buffer[ 38u ] = {
    'I', 'F', '0', '0', '0', '0', '0', '0', '0', '0', '0',  // Frequency (11 characters)
    '0', ' ', ' ', ' ', ' ', ' ',                           // Five spaces as reserved positions
    '+', '0', '0', '0', '0',                                // RIT/XIT offset (5 characters: sign and 4 digits)
    '0', '0', '0', '0', '0',                                // RIT/XIT parameters (5 characters)
    '0', '0', '0', '0', '0',                                // Additional parameters (P4–P13 placeholders)
    '0', '0', '0', ';'                                      // P14–P15 placeholders and terminating character
  };

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
  cat_answer_buffer[ 13 ] = convert_to_char( cat_step_frequency_1kHz );
  cat_answer_buffer[ 14 ] = convert_to_char( cat_step_frequency_100Hz );
  cat_answer_buffer[ 15 ] = convert_to_char( cat_step_frequency_10Hz );
  cat_answer_buffer[ 16 ] = convert_to_char( cat_step_frequency_1Hz );
  cat_answer_buffer[ 17 ] = cat_answer_rit_xit_18( cat_rit_frequency, cat_xit_frequency );
  if ( cat_RIT )
  {
    cat_answer_buffer[ 18 ] = convert_to_char( cat_rit_frequency_10kHz );
    cat_answer_buffer[ 19 ] = convert_to_char( cat_rit_frequency_1kHz );
    cat_answer_buffer[ 20 ] = convert_to_char( cat_rit_frequency_100Hz );
    cat_answer_buffer[ 21 ] = convert_to_char( cat_rit_frequency_10Hz );
    cat_answer_buffer[ 22 ] = convert_to_char( cat_rit_frequency_1Hz );
  }
  else if ( cat_XIT )
  {
    cat_answer_buffer[ 18 ] = convert_to_char( cat_xit_frequency_10kHz );
    cat_answer_buffer[ 19 ] = convert_to_char( cat_xit_frequency_1kHz );
    cat_answer_buffer[ 20 ] = convert_to_char( cat_xit_frequency_100Hz );
    cat_answer_buffer[ 21 ] = convert_to_char( cat_xit_frequency_10Hz );
    cat_answer_buffer[ 22 ] = convert_to_char( cat_xit_frequency_1Hz );
  }
  else
  {
    cat_answer_buffer[ 18 ] = convert_to_char( 0 );
    cat_answer_buffer[ 19 ] = convert_to_char( 0 );
    cat_answer_buffer[ 20 ] = convert_to_char( 0 );
    cat_answer_buffer[ 21 ] = convert_to_char( 0 );
    cat_answer_buffer[ 22 ] = convert_to_char( 0 );
  }

  cat_answer_buffer[ 23 ] = convert_to_char( cat_RIT );  // P4 0: RIT OFF, 1: RIT ON
  cat_answer_buffer[ 24 ] = convert_to_char( cat_XIT );  // P5 0: XIT OFF, 1: XIT ON
  cat_answer_buffer[ 25 ] = convert_to_char( 0 );        // P6 channel bank number
  cat_answer_buffer[ 26 ] = convert_to_char( 0 );        // P7 channel bank number
  cat_answer_buffer[ 27 ] = convert_to_char( 0 );        // P7 channel bank number
  cat_answer_buffer[ 28 ] = convert_to_char( 0 );        // P8 0: RX, 1: TX
  cat_answer_buffer[ 29 ] = convert_to_char( cat_MODE );        // P9 Operating mode. See MD commands for details
  cat_answer_buffer[ 30 ] = convert_to_char( 0 );        // P10 See FR and FT commands
  cat_answer_buffer[ 31 ] = convert_to_char( 0 );        // P11 Scan status. See SC command.
  cat_answer_buffer[ 32 ] = convert_to_char( 0 );        // P12 Split operation status. See SP command
  cat_answer_buffer[ 33 ] = convert_to_char( 0 );        // P13 0: OFF, 1: TONE, 2: CTCSS, 3: DCS
  cat_answer_buffer[ 34 ] = convert_to_char( 0 );        // P14 Tone frequency. See TN command.
  cat_answer_buffer[ 35 ] = convert_to_char( 0 );        // P14 Tone frequency. See TN command.
  cat_answer_buffer[ 36 ] = convert_to_char( 0 );        // P15 Shift status. See OS command
  cat_answer_buffer[ 37 ] = ';';
  void ( *p_cat_write_answer_function )( const char*, size_t ) = callbacks.answer_function;
  if ( p_cat_write_answer_function ) p_cat_write_answer_function( cat_answer_buffer, 38u );
}

static int symbol_to_int( uint8_t symbol )
{
  int number = symbol - '0';  // ASCII '0' corresponds to decimal 48
  if ( ( number >= 0 ) && ( number <= 9 ) ) return number;
  return -1;
}

static void handle_FA_set_frequency( const uint8_t* frequency_text )
{
  // Array of pointers to frequency variables
  int* frequency_components[] = { &cat_frequency_10GHz, &cat_frequency_1GHz, &cat_frequency_100MHz,
                                  &cat_frequency_10MHz, &cat_frequency_1MHz, &cat_frequency_100kHz,
                                  &cat_frequency_10kHz, &cat_frequency_1kHz, &cat_frequency_100Hz,
                                  &cat_frequency_10Hz,  &cat_frequency_1Hz };

  // Iterate through frequency text and update frequency components
  for ( int i = 0; i < 11; i++ )
  {
    int number = symbol_to_int( frequency_text[ i ] );
    if ( ( number < 0 ) || ( number > 9 ) ) return;  // Invalid character, stop processing
    *frequency_components[ i ] = number;
  }

  char fa_answer[] = { 'F', 'A', ';' };
  void ( *p_cat_write_answer_function )( const char*, size_t ) = callbacks.answer_function;
  if ( p_cat_write_answer_function ) p_cat_write_answer_function( fa_answer, 3u );

  vfo_a_frequency = calculate_frequency();
  void ( *set_new_frequency_vfo_a )( uint32_t ) = callbacks.set_frequency_vfo_a;
  if ( set_new_frequency_vfo_a ) set_new_frequency_vfo_a( vfo_a_frequency );
}

static void handle_MD_set_mode( const uint8_t* mode_text )
{
  // Convert the mode text to an integer value
  int number = symbol_to_int( mode_text[ 0 ] );
  if ( ( number < 1 ) || ( number > 9 ) ) return;  // Invalid character, stop processing

  // Set the mode based on the received value
  cat_MODE = number;

  // Prepare the response buffer
  char md_answer_buffer[ 4u ] = { 'M', 'D', '0', ';' };
  md_answer_buffer[ 2 ] = convert_to_char( cat_MODE );

  // Send the response using the callback function
  void ( *p_cat_write_answer_function )( const char*, size_t ) = callbacks.answer_function;
  if ( p_cat_write_answer_function ) p_cat_write_answer_function( md_answer_buffer, 4u );

  // Call the callback function to set the mode
  void ( *set_trx_mode )( uint8_t ) = callbacks.set_trx_mode;
  if ( set_trx_mode ) set_trx_mode( cat_MODE );
}

// read mode
static void handle_MD_query_mode( const uint8_t* )
{
  char md_answer_buffer[ 4u ] = { 'M', 'D', '0', ';' };
  md_answer_buffer[ 2 ] = convert_to_char( cat_MODE );
  void ( *p_cat_write_answer_function )( const char*, size_t ) = callbacks.answer_function;
  if ( p_cat_write_answer_function ) p_cat_write_answer_function( md_answer_buffer, 4u );
}

static void handle_FA_query_frequency( const uint8_t* )
{
  // Define the response buffer for FA; in the format FAxxxxxxxxxxx;
  char fa_answer_buffer[ 15u ] = { 'F', 'A' };

  // Array of frequency components to construct the response
  int frequency_components[] = { cat_frequency_10GHz, cat_frequency_1GHz,   cat_frequency_100MHz, cat_frequency_10MHz,
                                 cat_frequency_1MHz,  cat_frequency_100kHz, cat_frequency_10kHz,  cat_frequency_1kHz,
                                 cat_frequency_100Hz, cat_frequency_10Hz,   cat_frequency_1Hz };

  // Populate the frequency components into the response buffer
  for ( int i = 0; i < 11; i++ )
  {
    fa_answer_buffer[ 2 + i ] = convert_to_char( frequency_components[ i ] );
  }

  // Append the termination character
  fa_answer_buffer[ 13 ] = ';';

  // Write the response using the callback
  void ( *p_cat_write_answer_function )( const char*, size_t ) = callbacks.answer_function;
  if ( p_cat_write_answer_function )
  {
    p_cat_write_answer_function( fa_answer_buffer, 14 );
  }
}

static void handle_ID_query( const uint8_t* )
{
  // Define the response for the ID command
  const char id_response[] = { 'I', 'D', '0', '1', '9', ';' };

  // Call the answer callback to send the response
  if ( callbacks.answer_function )
  {
    callbacks.answer_function( id_response, 6 );
  }
}

// Command definitions
static const uint8_t CMD_IF[] = { 'I', 'F', ';' };
static const uint8_t CMD_FA_SET[] = { 'F', 'A' };         // FAxxxxxxxxxxx for setting frequency
static const uint8_t CMD_FA_QUERY[] = { 'F', 'A', ';' };  // FA; for querying frequency
static const uint8_t CMD_ID[] = { 'I', 'D', ';' };
static const uint8_t CMD_MD_SET[] = { 'M', 'D' };  // MDx for setting mode
static const uint8_t CMD_MD_QUERY[] = { 'M', 'D', ';' };

static commands_table_t command_table_release[] = {
  { CMD_IF, 3, handle_IF_query, 0 },                  //
  { CMD_FA_SET, 2, handle_FA_set_frequency, 12 },     //
  { CMD_FA_QUERY, 3, handle_FA_query_frequency, 0 },  //
  { CMD_ID, 3, handle_ID_query, 0 },                  //
  { CMD_MD_SET, 2, handle_MD_set_mode, 2 },           //
  { CMD_MD_QUERY, 3, handle_MD_query_mode, 0 }        //
};

#define COMMAND_TABLE_SIZE 6u

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

static uint8_t is_table_entry( const uint8_t* buffer, uint8_t input_buffer_length,
                               const commands_table_t* command_entry )
{
  const uint8_t* table_command = command_entry->command;
  uint8_t table_cmd_length = command_entry->command_length;
  uint8_t table_param_length = command_entry->param_length;

  uint8_t expected_buffer_length = table_cmd_length + table_param_length;
  if ( input_buffer_length == expected_buffer_length )
  {
    return buffer_and_command_are_same( buffer, table_command, table_cmd_length );
  }
  return 0;
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
    // Compare the received command with the command in the table
    if ( is_table_entry( buffer, buffer_length, &command_table[ i ] ) == 1 )
    {
      uint8_t cmd_length = command_table[ i ].command_length;
      command_table[ i ].handler( buffer + cmd_length );
    }
  }
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
    // cat_cmd_rx_buffer[ cat_cmd_rx_index ] = '\0';  // needs for tests
    cat_decode_received_cmd( cat_cmd_rx_buffer, cat_cmd_rx_index, command_table_release,
                             COMMAND_TABLE_SIZE );  // Process the command
    cat_cmd_rx_index = 0;                           // Reset the index for the next command
  }
}

void cat_init( const CAT_T* p_init_struct )
{
  callbacks.answer_function = p_init_struct->answer_function;
  callbacks.set_frequency_vfo_a = p_init_struct->set_frequency_vfo_a;
  callbacks.set_trx_mode = p_init_struct->set_trx_mode;
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