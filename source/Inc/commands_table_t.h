#ifndef COMMANDS_TABLE_T_H_
#define COMMANDS_TABLE_T_H_

#include <stdio.h>

typedef struct
{
  const uint8_t* command;                      // Pointer to the command bytes
  uint8_t command_length;                      // Length of the command
  void ( *handler )( const uint8_t* params );  // Handler for the command
  uint8_t param_length;                        // Expected parameter length (0 if none required)
} commands_table_t;

#endif  // COMMANDS_TABLE_T_H_