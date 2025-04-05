#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <libcat.h>  // Include your header file for cat_receive_cmd and cat_init
#include <tests/md_tests.h>

#define CAT_CMD_MAX_LENGTH 20u  // Maximum length of the command buffer

// Mock callback functions and test variables
static char test_answer_buffer[ CAT_CMD_MAX_LENGTH ];
static uint8_t tests_trx_mode = 0;
static uint8_t mock_answer_called = 0;
static uint8_t mock_mode_set_called = 0;

// Mock implementation of the answer callback
static void MockAnswerCallback( const char* answer, size_t length )
{
  mock_answer_called = 1;
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  memcpy( test_answer_buffer, answer, length );
}

// Mock implementation of the mode setting callback
static void MockSetModeCallback( uint8_t mode )
{
  mock_mode_set_called = 1;
  tests_trx_mode = mode;
}

// Reset mock states before each test
static void ResetMocks( void )
{
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  tests_trx_mode = 0;
  mock_answer_called = 0;
  mock_mode_set_called = 0;
}

// Test: MD command sets mode correctly
static void test_MD_command_sets_mode( void )
{
  ResetMocks();

  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = MockSetModeCallback };

  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Simulate sending the MD command to set mode
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( '4' );  // Set mode to LSB (P1 = 4)
  cat_receive_cmd( ';' );

  // Verify the mode setting callback was called
  TEST_ASSERT_TRUE( mock_mode_set_called );
  TEST_ASSERT_EQUAL_UINT8( 4, tests_trx_mode );  // Mode should be set to LSB (P1 = 1)

  // Verify the answer callback was called with the correct response
  // TEST_ASSERT_TRUE( mock_answer_called );
  // TEST_ASSERT_EQUAL_STRING( "MD4;", test_answer_buffer );
}

// Test: MD command with invalid characters
void test_MD_command_invalid_characters( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = MockSetModeCallback };

  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Simulate sending an invalid MD command
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( 'X' );  // Invalid character
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the mode setting callback was not called
  TEST_ASSERT_FALSE( mock_mode_set_called );
}

// Test: MD command processes partial mode input
void test_MD_command_partial_mode( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = MockSetModeCallback };
  cat_init( &init_struct );

  // Simulate sending a partial MD command with extra digits
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( '1' );  // Valid mode digit
  cat_receive_cmd( '2' );  // Extra digit (invalid)
  cat_receive_cmd( '3' );  // Extra digit (invalid)
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the mode setting callback was not called
  TEST_ASSERT_FALSE( mock_mode_set_called );
}

// Test: MD command with invalid P1 argument (0)
void test_MD_command_invalid_argument( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = MockSetModeCallback };
  cat_init( &init_struct );

  // Simulate sending an MD command with an invalid P1 argument (0)
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( '0' );  // Invalid mode digit
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the mode setting callback was not called
  TEST_ASSERT_FALSE( mock_mode_set_called );
}

// Test: MD; command returns the correct mode
void test_MD_query_command_response( void )
{
  ResetMocks();

  // Initialize the CAT system with the mock callback
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = MockSetModeCallback };
  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Set the mode using the MD command
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( '6' );  // Set mode to FSK (P1 = 6)
  cat_receive_cmd( ';' );

  // Reset the mock state to prepare for the MD; query test
  ResetMocks();

  // Simulate querying the mode with the MD; command
  cat_receive_cmd( 'M' );
  cat_receive_cmd( 'D' );
  cat_receive_cmd( ';' );

  // Expected response buffer for MD; command
  const char expected_response[] = { 'M', 'D', '6', ';' };

  // Verify the response buffer and callback invocation
  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_MEMORY( expected_response, test_answer_buffer, 4 );
}

// Test runner for this file
void run_md_tests( void )
{
  RUN_TEST( test_MD_query_command_response );
  // FA tests SET
  RUN_TEST( test_MD_command_sets_mode );
  RUN_TEST( test_MD_command_invalid_characters );
  RUN_TEST( test_MD_command_partial_mode );
  RUN_TEST( test_MD_command_invalid_argument );
}
