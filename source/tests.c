#include <unity.h>
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

// Основна функція тестування
int main( void )
{
  UNITY_BEGIN();

  RUN_TEST( test_receive_full_command );
  RUN_TEST( test_receive_multiple_commands );
  RUN_TEST( test_buffer_overflow );
  RUN_TEST( test_incomplete_command );
  RUN_TEST( test_reset_after_processing );

  return UNITY_END();
}
