#ifndef LIBCAT_H_
#define LIBCAT_H_

#ifdef __cplusplus
extern "C"
{
#endif
#define CAT_CMD_MAX_LENGTH 16u

  void cat_receive_cmd( char );

#if defined( TESTS )
  char* get_cat_cmd_rx_buffer( void );
  uint8_t get_cat_cmd_rx_index( void );
#endif

#ifdef __cplusplus
}
#endif

#endif  // LIBCAT_H_
