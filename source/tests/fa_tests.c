#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <libcat.h>  // Include your header file for cat_receive_cmd and cat_init
#include <tests/fa_tests.h>

// Mock callback functions and test variables
static char test_answer_buffer[ CAT_CMD_MAX_LENGTH ];
static uint32_t test_vfo_a_frequency = 0;
static uint8_t mock_answer_called = 0;
static uint8_t mock_frequency_set_called = 0;

// Mock implementation of the answer callback
static void MockAnswerCallback( const char* answer, size_t length )
{
  mock_answer_called = 1;
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  memcpy( test_answer_buffer, answer, length );
}

// Mock implementation of the frequency setting callback
static void MockSetFrequencyCallback( uint32_t frequency )
{
  mock_frequency_set_called = 1;
  test_vfo_a_frequency = frequency;
}

// Reset mock states before each test
static void ResetMocks( void )
{
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  test_vfo_a_frequency = 0;
  mock_answer_called = 0;
  mock_frequency_set_called = 0;
}

// Test: FA command processes frequency correctly
static void test_FA_command_sets_frequency( void )
{
  ResetMocks();

  CAT_T init_struct = { .answer_function = MockAnswerCallback, .set_frequency_vfo_a = MockSetFrequencyCallback };

  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  cat_receive_cmd( 'F' );
  cat_receive_cmd( 'A' );
  cat_receive_cmd( '0' );  // 10Ghz
  cat_receive_cmd( '0' );  // 1Ghz
  cat_receive_cmd( '0' );  // 100Mhz
  cat_receive_cmd( '1' );  // 10Mhz
  cat_receive_cmd( '4' );  // 1Mhz
  cat_receive_cmd( '2' );  // 100kHz
  cat_receive_cmd( '5' );  // 10kHz
  cat_receive_cmd( '0' );  // 1kHz
  cat_receive_cmd( '0' );  // 100Hz
  cat_receive_cmd( '0' );  // 10Hz
  cat_receive_cmd( '0' );  // 1Hz
  cat_receive_cmd( ';' );

  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_STRING( "FA;", test_answer_buffer );
  TEST_ASSERT_TRUE( mock_frequency_set_called );
  TEST_ASSERT_EQUAL_UINT32( 14250000, test_vfo_a_frequency );
}

// Test: FA command with invalid characters
void test_FA_command_invalid_characters( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback, .set_frequency_vfo_a = MockSetFrequencyCallback };

  cat_init( &init_struct );

  // Simulate sending an invalid FA command
  cat_receive_cmd( 'F' );
  cat_receive_cmd( 'A' );
  cat_receive_cmd( '0' );
  cat_receive_cmd( '0' );
  cat_receive_cmd( 'X' );  // Invalid character
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the frequency callback was not called
  TEST_ASSERT_FALSE( mock_frequency_set_called );
}

// Test: FA command processes partial frequency
void test_FA_command_partial_frequency( void )
{
  ResetMocks();

  // Initialize the CAT system with mock callbacks
  CAT_T init_struct = { .answer_function = MockAnswerCallback, .set_frequency_vfo_a = MockSetFrequencyCallback };
  cat_init( &init_struct );

  // Simulate sending a partial FA command
  cat_receive_cmd( 'F' );
  cat_receive_cmd( 'A' );
  cat_receive_cmd( '0' );
  cat_receive_cmd( '0' );
  cat_receive_cmd( '0' );
  cat_receive_cmd( ';' );

  // Verify the answer callback was not called
  TEST_ASSERT_FALSE( mock_answer_called );

  // Verify the frequency callback was not called
  TEST_ASSERT_FALSE( mock_frequency_set_called );
}

// Test: FA; command returns the correct frequency
void test_FA_query_command_response( void )
{
  ResetMocks();

  // Initialize the CAT system with the mock callback
  CAT_T init_struct = { .answer_function = MockAnswerCallback, .set_frequency_vfo_a = MockSetFrequencyCallback };
  cat_init( &init_struct );

  cat_receive_cmd( ';' );

  // Set the frequency using the FA command
  cat_receive_cmd( 'F' );
  cat_receive_cmd( 'A' );
  cat_receive_cmd( '0' );  // 10GHz
  cat_receive_cmd( '0' );  // 1GHz
  cat_receive_cmd( '0' );  // 100MHz
  cat_receive_cmd( '1' );  // 10MHz
  cat_receive_cmd( '4' );  // 1MHz
  cat_receive_cmd( '2' );  // 100kHz
  cat_receive_cmd( '5' );  // 10kHz
  cat_receive_cmd( '0' );  // 1kHz
  cat_receive_cmd( '0' );  // 100Hz
  cat_receive_cmd( '0' );  // 10Hz
  cat_receive_cmd( '0' );  // 1Hz
  cat_receive_cmd( ';' );

  // Reset the mock state to prepare for the FA; query test
  ResetMocks();

  // Simulate querying the frequency with the FA; command
  cat_receive_cmd( 'F' );
  cat_receive_cmd( 'A' );
  cat_receive_cmd( ';' );

  // Expected response buffer for FA; command
  const char expected_response[] = { 'F', 'A', '0', '0', '0', '1', '4', '2', '5', '0', '0', '0', '0', ';' };

  // Verify the response buffer and callback invocation
  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_MEMORY( expected_response, test_answer_buffer, 14 );
}

// Test runner for this file
void run_fa_tests( void )
{
  RUN_TEST( test_FA_query_command_response );
  // FA tests SET
  RUN_TEST( test_FA_command_sets_frequency );
  RUN_TEST( test_FA_command_invalid_characters );
  RUN_TEST( test_FA_command_partial_frequency );
}
