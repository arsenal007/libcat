
#include <unity.h>
#include <tests/set_tests.h>
#include <libcat.h>


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

void run_set_tests( void )
{
  // Add tests for test_set
  RUN_TEST( test_set_full );
  RUN_TEST( test_set_partial );
}