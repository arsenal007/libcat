#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <libcat.h>  // Include your header file for cat_receive_cmd and cat_init
#include <commands_table_t.h>
#include <tests/cat_decode_received_cmd_no_params_tests.h>
#include <tests/tests.h>  // Include your header file for test functions

#define CAT_CMD_MAX_LENGTH 20u  // Maximum length of the command buffer

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

void run_cat_decode_received_cmd_no_params_tests( void )
{
  // Add tests for test_cat_decode_received_cmd
  RUN_TEST( test_cat_decode_received_cmd_no_params );
  RUN_TEST( test_cat_decode_received_cmd_with_params );
  RUN_TEST( test_cat_decode_received_cmd_invalid_params );
  RUN_TEST( test_cat_decode_received_cmd_unknown_command );
  RUN_TEST( test_cat_decode_received_cmd_empty_buffer );
}