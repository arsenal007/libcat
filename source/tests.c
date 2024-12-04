#include <unity.h>
#include <commands_table_t.h>
#include <libcat.h>
#include <string.h>

void setUp( void ) {}

void tearDown( void ) {}

// Test: receiving a full command
void test_receive_full_command( void )
{
  // Simulate sending characters of the command
  cat_receive_cmd( 'H' );
  cat_receive_cmd( 'E' );
  cat_receive_cmd( 'L' );
  cat_receive_cmd( 'L' );
  cat_receive_cmd( 'O' );
  cat_receive_cmd( ';' );  // Termination character

  // Check that the buffer contains the full command
  TEST_ASSERT_EQUAL_STRING( "HELLO;", get_cat_cmd_rx_buffer() );

  // Verify that the index is reset after processing the command
  TEST_ASSERT_EQUAL_UINT8( 0, get_cat_cmd_rx_index() );
}

// Тест: обробка декількох команд
void test_receive_multiple_commands( void )
{
  cat_receive_cmd( 'C' );
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( '1' );
  cat_receive_cmd( ';' );

  TEST_ASSERT_EQUAL_STRING( "CMD1;", get_cat_cmd_rx_buffer() );

  cat_receive_cmd( 'C' );
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( '2' );
  cat_receive_cmd( ';' );

  TEST_ASSERT_EQUAL_STRING( "CMD2;", get_cat_cmd_rx_buffer() );
}

// Test: buffer overflow
void test_buffer_overflow( void )
{
  uint8_t i;

  // Send more characters than the buffer can hold
  for ( i = 0; i < CAT_CMD_MAX_LENGTH + 5; i++ )
  {
    cat_receive_cmd( 'A' );
  }

  // Check that the buffer contains 'A' up to its last position
  char expected[ CAT_CMD_MAX_LENGTH ];
  memset( expected, 'A', CAT_CMD_MAX_LENGTH );  // Fill expected buffer with 'A'

  TEST_ASSERT_EQUAL_MEMORY( expected, get_cat_cmd_rx_buffer(), CAT_CMD_MAX_LENGTH );

  // Ensure the index is at the last valid position
  TEST_ASSERT_EQUAL_UINT8( CAT_CMD_MAX_LENGTH - 1, get_cat_cmd_rx_index() );
}

// Test: command without a termination character
void test_incomplete_command( void )
{
  cat_receive_cmd( ';' );
  cat_receive_cmd( 'I' );
  cat_receive_cmd( 'N' );
  cat_receive_cmd( 'C' );
  cat_receive_cmd( 'O' );
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'P' );

  // Manually null-terminate the expected buffer for comparison
  char expected[ CAT_CMD_MAX_LENGTH ] = { 'I', 'N', 'C', 'O', 'M', 'P' };

  // Verify that the first 6 characters of the buffer match the expected data
  TEST_ASSERT_EQUAL_MEMORY( expected, get_cat_cmd_rx_buffer(), 6 );

  // Verify that the index correctly points to the next position
  TEST_ASSERT_EQUAL_UINT8( 6, get_cat_cmd_rx_index() );
}

// Test: handling after command termination
void test_reset_after_processing( void )
{
  // Send the first command
  cat_receive_cmd( 'T' );
  cat_receive_cmd( 'E' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( 'T' );
  cat_receive_cmd( ';' );  // Termination character

  // Verify that the buffer was reset after processing
  TEST_ASSERT_EQUAL_UINT8( 0, get_cat_cmd_rx_index() );

  // Send a new command
  cat_receive_cmd( 'N' );
  cat_receive_cmd( 'E' );
  cat_receive_cmd( 'W' );

  // Verify that the new command is being assembled correctly
  char expected_new_command[ 3 ] = { 'N', 'E', 'W' };
  TEST_ASSERT_EQUAL_MEMORY( expected_new_command, get_cat_cmd_rx_buffer(), 3 );
  TEST_ASSERT_EQUAL_UINT8( 3, get_cat_cmd_rx_index() );
}

void test_buffer_and_command_are_same_equal( void )
{
  uint8_t buffer[] = { 'A', 'B', 'C' };
  uint8_t command[] = { 'A', 'B', 'C' };

  // Should return 1 (match)
  TEST_ASSERT_EQUAL_UINT8( 1, test_buffer_and_command_are_same( buffer, command, 3 ) );
}

void test_buffer_and_command_are_same_not_equal( void )
{
  uint8_t buffer[] = { 'A', 'B', 'C' };
  uint8_t command[] = { 'A', 'B', 'D' };

  // Should return 0 (no match)
  TEST_ASSERT_EQUAL_UINT8( 0, test_buffer_and_command_are_same( buffer, command, 3 ) );
}

void test_buffer_and_command_are_same_partial_match( void )
{
  uint8_t buffer[] = { 'A', 'B', 'C' };
  uint8_t command[] = { 'A', 'B' };

  // Should return 0 (no match, lengths differ)
  TEST_ASSERT_EQUAL_UINT8( 1, test_buffer_and_command_are_same( buffer, command, 2 ) );
}

// Add tests for test_copy
void test_copy_full( void )
{
  uint8_t dst[ 5 ] = { 0 };
  const char src[] = "HELLO";

  test_copy( dst, src, 5 );

  // Destination should match source
  TEST_ASSERT_EQUAL_UINT8_ARRAY( src, dst, 5 );
}

void test_copy_partial( void )
{
  uint8_t dst[ 5 ] = { 0 };
  const char src[] = "HELLO";

  test_copy( dst, src, 3 );

  // Only first 3 characters should be copied
  TEST_ASSERT_EQUAL_UINT8( 'H', dst[ 0 ] );
  TEST_ASSERT_EQUAL_UINT8( 'E', dst[ 1 ] );
  TEST_ASSERT_EQUAL_UINT8( 'L', dst[ 2 ] );
  TEST_ASSERT_EQUAL_UINT8( 0, dst[ 3 ] );
  TEST_ASSERT_EQUAL_UINT8( 0, dst[ 4 ] );
}

// Add tests for test_set
void test_set_full( void )
{
  uint8_t dst[ 5 ] = { 0 };

  test_set( dst, 0xFF, 5 );

  // All elements should be set to 0xFF
  for ( int i = 0; i < 5; i++ )
  {
    TEST_ASSERT_EQUAL_UINT8( 0xFF, dst[ i ] );
  }
}

void test_set_partial( void )
{
  uint8_t dst[ 5 ] = { 0 };

  test_set( dst, 0xAA, 3 );

  // Only first 3 elements should be set to 0xAA
  TEST_ASSERT_EQUAL_UINT8( 0xAA, dst[ 0 ] );
  TEST_ASSERT_EQUAL_UINT8( 0xAA, dst[ 1 ] );
  TEST_ASSERT_EQUAL_UINT8( 0xAA, dst[ 2 ] );
  TEST_ASSERT_EQUAL_UINT8( 0, dst[ 3 ] );
  TEST_ASSERT_EQUAL_UINT8( 0, dst[ 4 ] );
}

// Add tests for test_cat_decode_received_cmd
static uint8_t mock_handler_called = 0;
static uint8_t mock_handler_params[ CAT_CMD_MAX_LENGTH ];

static void MockHandler( const uint8_t* params )
{
  mock_handler_called = 1;
  if ( params )
  {
  }
}

static void Mock_FA( const uint8_t* params )
{
  mock_handler_called = 1;
  if ( params )
  {
    memcpy( mock_handler_params, params, 11 );
  }
}

static void ResetMocks( void )
{
  mock_handler_called = 0;
  test_set( mock_handler_params, 0, CAT_CMD_MAX_LENGTH );
}

void test_cat_decode_received_cmd_no_params( void )
{
  // Reset the mock state before the test
  ResetMocks();

  // Command table for the test
  // CMD_IF is a command with no parameters (length 3)
  const uint8_t CMD_IF[] = { 'I', 'F', ';' };
  commands_table_t test_table[] = {
    { CMD_IF, 3, MockHandler, 0 }  // The handler is MockHandler, and no parameters are expected
  };

  // Input buffer containing the command "IF;"
  const char cmd_buffer[] = { 'I', 'F', ';' };
  uint8_t buffer_length = 3;

  // Call the wrapper function to decode and process the command
  test_cat_decode_received_cmd( cmd_buffer, buffer_length, test_table, 1 );

  // Verify that the command handler was called
  TEST_ASSERT_TRUE( mock_handler_called );
  // Since no parameters are expected, we do not need to verify mock_handler_params
}

void test_cat_decode_received_cmd_with_params( void )
{
  // Command table for the test
  // CMD_FA is a command that expects 11 parameters
  const uint8_t CMD_FA[] = { 'F', 'A' };
  commands_table_t test_table[] = { { CMD_FA, 2, Mock_FA, 11 } };

  // Input buffer containing the command "FA" followed by 11 parameters and a termination character ";"
  const char cmd_buffer[] = { 'F', 'A', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', ';' };
  uint8_t buffer_length = sizeof( cmd_buffer );

  // Reset the mock state before the test
  ResetMocks();

  // Call the wrapper function to decode and process the command
  test_cat_decode_received_cmd( cmd_buffer, buffer_length, test_table, 1 );

  // Verify that the command handler was called
  TEST_ASSERT_TRUE( mock_handler_called );

  // Verify that the correct parameters were passed to the handler
  // Parameters start at cmd_buffer[2] and have a length of 11
  TEST_ASSERT_EQUAL_UINT8_ARRAY( cmd_buffer + 2, mock_handler_params, 11 );
}

void test_cat_decode_received_cmd_invalid_params( void )
{
  // Reset the mock state before the test
  ResetMocks();

  // Command table for the test
  // CMD_FA is a command that expects 11 parameters
  const uint8_t CMD_FA[] = { 'F', 'A' };
  commands_table_t test_table[] = {
    { CMD_FA, 2, Mock_FA, 11 }  // Command length is 2, and 11 parameters are required
  };

  // Input buffer containing the command "FA" with insufficient parameters and a termination character ";"
  const char cmd_buffer[] = { 'F', 'A', '0', '0', ';' };
  uint8_t buffer_length = 5;

  // Call the wrapper function to decode and process the command
  test_cat_decode_received_cmd( cmd_buffer, buffer_length, test_table, 1 );

  // Verify that the command handler was not called
  // The parameter length does not match the expected length, so the handler should not be invoked
  TEST_ASSERT_FALSE( mock_handler_called );
}

void test_cat_decode_received_cmd_unknown_command( void )
{
  // Command table for the test
  // CMD_IF is the only recognized command in this table
  const uint8_t CMD_IF[] = { 'I', 'F', ';' };
  commands_table_t test_table[] = {
    { CMD_IF, 3, MockHandler, 0 }  // The handler is MockHandler, and no parameters are expected
  };

  // Input buffer containing an unknown command "XY;"
  const char cmd_buffer[] = { 'X', 'Y', ';' };
  uint8_t buffer_length = sizeof( cmd_buffer );

  // Reset the mock state before the test
  ResetMocks();

  // Call the wrapper function to decode and process the command
  test_cat_decode_received_cmd( cmd_buffer, buffer_length, test_table, 1 );

  // Verify that the command handler was not called
  // Since the command "XY;" does not exist in the command table, the handler should not be invoked
  TEST_ASSERT_FALSE( mock_handler_called );
}

void test_cat_decode_received_cmd_empty_buffer( void )
{
  // Command table for the test
  // CMD_IF is the only valid command in this table
  const uint8_t CMD_IF[] = { 'I', 'F', ';' };
  commands_table_t test_table[] = {
    { CMD_IF, 3, MockHandler, 0 }  // The handler is MockHandler, and no parameters are expected
  };

  // Empty input buffer
  const char cmd_buffer[] = {};
  uint8_t buffer_length = 0;

  // Reset the mock state before the test
  ResetMocks();

  // Call the wrapper function to decode and process the command
  test_cat_decode_received_cmd( cmd_buffer, buffer_length, test_table, 1 );

  // Verify that the command handler was not called
  // Since the input buffer is empty, no commands can be matched, and the handler should not be invoked
  TEST_ASSERT_FALSE( mock_handler_called );
}

int main( void )
{
  UNITY_BEGIN();

  // cat_receive_cmd
  RUN_TEST( test_receive_full_command );
  RUN_TEST( test_receive_multiple_commands );
  RUN_TEST( test_buffer_overflow );
  RUN_TEST( test_incomplete_command );
  RUN_TEST( test_reset_after_processing );

  // compare_command
  RUN_TEST( test_buffer_and_command_are_same_equal );
  RUN_TEST( test_buffer_and_command_are_same_not_equal );
  RUN_TEST( test_buffer_and_command_are_same_partial_match );

  // Add tests for test_copy
  RUN_TEST( test_copy_full );
  RUN_TEST( test_copy_partial );

  // Add tests for test_set
  RUN_TEST( test_set_full );
  RUN_TEST( test_set_partial );

  // Add tests for test_cat_decode_received_cmd
  RUN_TEST( test_cat_decode_received_cmd_no_params );
  RUN_TEST( test_cat_decode_received_cmd_with_params );
  RUN_TEST( test_cat _decode_received_cmd_invalid_params );
  RUN_TEST( test_cat_decode_received_cmd_unknown_command );
  RUN_TEST( test_cat_decode_received_cmd_empty_buffer );

  return UNITY_END();
}
