#ifndef LIBCAT_H_
#define LIBCAT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

  void cat_receive_cmd( char );

  typedef struct
  {
    void ( *answer_function )( const char*, size_t );
    void ( *set_frequency_vfo_a )( uint32_t );
    void ( *set_trx_mode )( uint8_t );
    void ( *set_key_speed_words_per_minute )( uint8_t );
  } CAT_T;

  void cat_init( const CAT_T* );
  
#ifdef __cplusplus
}
#endif

#endif  // LIBCAT_H_
