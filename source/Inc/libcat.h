#ifndef LIBCAT_H_
#define LIBCAT_H_

#ifdef __cplusplus
extern "C"
{
#endif
#define CAT_CMD_MAX_LENGTH 16u

  void cat_receive_cmd( char );

  void cat_init( void ( * )( const char*, size_t ) );

#if defined( TESTS )
  char* get_cat_cmd_rx_buffer( void );
  uint8_t get_cat_cmd_rx_index( void );
  uint8_t test_buffer_and_command_are_same( const uint8_t*, const uint8_t*, uint8_t );
  void test_copy( uint8_t*, const char*, uint8_t );
  void test_set( uint8_t*, uint8_t, uint8_t );
  typedef struct commands_table_t commands_table_t;
  void test_cat_decode_received_cmd( const char*, uint8_t buffer_length, const commands_table_t*, uint8_t );
#endif

#ifdef __cplusplus
}
#endif

#endif  // LIBCAT_H_
