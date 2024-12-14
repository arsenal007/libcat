#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <libcat.h>
#include <tests/if_tests.h>

// Mock variables for capturing the test state
static char test_answer_buffer[ CAT_CMD_MAX_LENGTH ];
static uint8_t mock_answer_called = 0;

// Mock implementation of the answer callback
static void MockAnswerCallback( const char* answer, size_t length )
{
  mock_answer_called = 1;
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  memcpy( test_answer_buffer, answer, length );
}

static uint8_t mock_frequency_set_called = 0;

static uint32_t test_vfo_a_frequency = 0;

// Mock implementation of the frequency setting callback
static void MockSetFrequencyCallback( uint32_t frequency )
{
  mock_frequency_set_called = 1;
  test_vfo_a_frequency = frequency;
}

// Reset the mock state before each test
static void ResetMocks( void )
{
  memset( test_answer_buffer, 0, CAT_CMD_MAX_LENGTH );
  mock_answer_called = 0;
}

// Test: IF command provides correct response (focus on frequency reporting)
static void test_IF_command_response( void )
{
  ResetMocks();

  // Initialize the CAT system with the mock callback
  CAT_T init_struct = {
    .answer_function = MockAnswerCallback,
    .set_frequency_vfo_a = MockSetFrequencyCallback  // Required for FA command
  };
  cat_init( &init_struct );

  // Simulate sending the FA command to set the frequency
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

  // Verify that the frequency was set correctly by the FA command
  TEST_ASSERT_TRUE( mock_frequency_set_called );
  TEST_ASSERT_EQUAL_UINT32( 14250000, test_vfo_a_frequency );

  // Reset the mock state to test the IF command
  ResetMocks();

  // Simulate sending the IF command to query the current frequency
  cat_receive_cmd( 'I' );
  cat_receive_cmd( 'F' );
  cat_receive_cmd( ';' );

  // Expected response buffer for the IF command
  const char expected_response[] = {
    'I',   //  0
    'F',   //  1
    '0',   //  2
    '0',   //  3
    '0',   //  4
    '1',   //  5
    '4',   //  6
    '2',   //  7
    '5',   //  8
    '0',   //  9
    '0',   // 10
    '0',   // 11
    '0',   // 12
    '0',   // 13
    '0',   // 14
    '0',   // 15
    '0',   // 16
    ' ',   // 17 Placeholder space
    '0',   // 18 Placeholder for RIT/XIT offset
    '0',   // 19
    '0',   // 20
    '0',   // 21
    '0',   // 22 Placeholder for additional RIT/XIT parameters
    '0',   // 23
    '0',   // 24
    '0',   // 25
    '0',   // 26
    '0',   // 27
    '0',   // 28 Placeholder for other parameters (P4–P13)
    '0',   // 29
    '0',   // 30
    '0',   // 31
    '0',   // 32
    '0',   // 33
    '0',   // 34
    '0',   // 35
    '0',   // 36
    ';'    // 37 Termination character
};

  // Verify the response buffer and callback invocation for the IF command
  TEST_ASSERT_TRUE( mock_answer_called );
  TEST_ASSERT_EQUAL_MEMORY( expected_response, test_answer_buffer, sizeof( expected_response ) );
}

void run_if_tests( void )
{
  RUN_TEST( test_IF_command_response );
}