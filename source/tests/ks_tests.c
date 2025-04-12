#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <libcat.h>  // Include your header file for cat_receive_cmd and cat_init
#include <tests/ks_tests.h>

#define CAT_CMD_MAX_LENGTH 20u  // Maximum length of the command buffer

// Mock callback functions and test variables
static char test_answer_buffer[ CAT_CMD_MAX_LENGTH ];
static uint8_t tests_key_speed = 0;
static uint8_t mock_answer_called = 0;
static uint8_t mock_key_speed_set_called = 0;

// Mock implementation of the answer callback
static void MockAnswerCallback( const char* answer, size_t length )
{
  mock_answer_called = 1;
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  memcpy( test_answer_buffer, answer, length );
}

// Mock implementation of the mode setting callback
static void MockSetKeySpeedCallback( uint8_t key_speed )
{
  mock_key_speed_set_called= 1;
  tests_key_speed = key_speed;
}

// Reset mock states before each test
static void ResetMocks( void )
{
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  tests_key_speed = 0;
  mock_answer_called = 0;
  mock_key_speed_set_called = 0;
}

// Test: KS command sets key speed correctly
static void test_KS_command_sets_key_speed( void )
{
  ResetMocks();

  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = NULL,
                        .set_key_speed_words_per_minute = MockSetKeySpeedCallback };

  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Simulate sending the KS command to set key speed
  cat_receive_cmd( 'K' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( '0' );  // Set key speed to 1
  cat_receive_cmd( '2' );  // Set key speed to 12
  cat_receive_cmd( '1' );
  cat_receive_cmd( ';' );

  // Verify the key speed setting callback was called
  TEST_ASSERT_TRUE( mock_key_speed_set_called );
  TEST_ASSERT_EQUAL_UINT8( 21, tests_key_speed );  // Key speed should be set to 12

  // Verify the answer callback was called with the correct response
  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_STRING( "KS021;", test_answer_buffer );
}

// Test: KS command with invalid characters
void test_KS_command_invalid_characters( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = NULL,
                        .set_key_speed_words_per_minute = MockSetKeySpeedCallback };

  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Simulate sending an invalid KS command
  cat_receive_cmd( 'K' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( 'X' );  // Invalid character
  cat_receive_cmd( '1' );
  cat_receive_cmd( '0' );
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the key speed setting callback was not called
  TEST_ASSERT_FALSE( mock_key_speed_set_called );
}

// Test: KS command processes partial speed input
void test_KS_command_partial_speed( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = NULL,
                        .set_key_speed_words_per_minute = MockSetKeySpeedCallback };
  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Simulate sending a partial KS command with extra digits
  cat_receive_cmd( 'K' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( '0' );  // Partial speed input
  cat_receive_cmd( '1' );  // Partial speed input
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the key speed setting callback was not called
  TEST_ASSERT_FALSE( mock_key_speed_set_called );
}

// Test: KS command with invalid speed argument (0)
void test_KS_command_invalid_argument( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = NULL,
                        .set_key_speed_words_per_minute = MockSetKeySpeedCallback };
  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Simulate sending a KS command with an invalid speed argument (0)
  cat_receive_cmd( 'K' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( '0' );  
  cat_receive_cmd( '0' );  
  cat_receive_cmd( '0' );  
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the key speed setting callback was not called
  TEST_ASSERT_FALSE( mock_key_speed_set_called );
}

// Test: KS; command returns the correct key speed
void test_KS_query_command_response( void )
{
  ResetMocks();

  // Initialize the CAT system with the mock callback
  CAT_T init_struct = { .answer_function = MockAnswerCallback,
                        .set_frequency_vfo_a = NULL,
                        .set_trx_mode = NULL,
                        .set_key_speed_words_per_minute = MockSetKeySpeedCallback };
                        
  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Set the key speed using the KS command
  cat_receive_cmd( 'K' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( '0' );  
  cat_receive_cmd( '3' );  // Set key speed to 21
  cat_receive_cmd( '1' );
  cat_receive_cmd( ';' );

  // Reset the mock state to prepare for the KS; query test
  ResetMocks();

  // Simulate querying the key speed with the KS; command
  cat_receive_cmd( 'K' );
  cat_receive_cmd( 'S' );
  cat_receive_cmd( ';' );

  // Expected response buffer for KS; command
  const char expected_response[] = { 'K', 'S', '0','3', '1', ';' };

  // Verify the response buffer and callback invocation
  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_MEMORY( expected_response, test_answer_buffer, 5 );
}

// Test runner for this file
void run_ks_tests( void )
{
  RUN_TEST( test_KS_command_sets_key_speed );
  RUN_TEST( test_KS_command_invalid_characters );
  RUN_TEST( test_KS_command_partial_speed );
  RUN_TEST( test_KS_command_invalid_argument );
  RUN_TEST( test_KS_query_command_response );
}
