#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <libcat.h>
#include <tests/id_tests.h>

#define CAT_CMD_MAX_LENGTH 20u  // Maximum length of the command buffer

// Mock callback functions and test variables
static char test_answer_buffer[ CAT_CMD_MAX_LENGTH ];
static uint8_t mock_answer_called = 0;

// Mock implementation of the answer callback
static void MockAnswerCallback( const char* answer, size_t length )
{
  mock_answer_called = 1;
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  memcpy( test_answer_buffer, answer, length );
}

// Reset the mock state before each test
static void ResetMocks( void )
{
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  mock_answer_called = 0;
}

void test_ID_command_response( void )
{
  ResetMocks();

  // Initialize the CAT system with the mock callback
  CAT_T init_struct = {
    .answer_function = MockAnswerCallback,
    .set_frequency_vfo_a = NULL  // Not needed for ID; tests
  };
  cat_init( &init_struct );

  // Simulate sending the ID; command
  cat_receive_cmd( 'I' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( ';' );

  // Expected response buffer for the ID; command
  const char expected_response[] = { 'I', 'D', '0', '1', '9', ';' };

  // Verify the response buffer and callback invocation
  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_MEMORY( expected_response, test_answer_buffer, sizeof( expected_response ) );
}

void run_id_tests( void )
{
  RUN_TEST( test_ID_command_response );
}