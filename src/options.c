#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"
#include "client.h"

void create_options(char *rrq_ptr, int rrq_raw_len, struct OPTION options[], int option_count)
{
  int str_place = rrq_raw_len;
  int i;
  for (i = 0; i < option_count; i++)
  {
    strncpy(rrq_ptr + str_place, options[i].name, strlen(options[i].name));
    str_place += strlen(options[i].name) + 1;
    strncpy(rrq_ptr + str_place, options[i].value, strlen(options[i].value));
    str_place += strlen(options[i].value) + 1;
  }
}

int find_option_length(struct OPTION options[], int option_count)
{

  if (!option_count)
    return 0;

  int option_length = 0;
  int i;
  for (i = 0; i < option_count; i++)
  {
    if (!options[i].silent)
    {
      option_length += strlen(options[i].name) + 1;
      option_length += strlen(options[i].value) + 1;
    }
  }

  return option_length;
}

void set_option_flags(struct OPTION options[], int option_count, int *blocksize_ptr, int *windowsize_ptr)
{
  int i;
  for (i = 0; i < option_count; i++)
  {
    if (strcmp(options[i].name, BLOCKSIZE_FLAG) == 0)
    {
      *blocksize_ptr = (int)strtol(options[i].value, (char **)NULL, 10);
    }
    else if (strcmp(options[i].name, WINDOWSIZE_FLAG) == 0)
    {
      *windowsize_ptr = (int)strtol(options[i].value, (char **)NULL, 10);
    }
    else
    {
      print_if_verbose("Error: unrecognized option '%s'\n", options[i].name);
      exit(1);
    }
  }
}

int process_oack(char *oack, int *blocksize_ptr, int *windowsize_ptr)
{
  int str_place = OPCODE_SIZE;

  for (;;)
  {

    if (str_place > OACK_MAX_SIZE + 4 || strlen(oack + str_place) == 0)
      break;

    char option[strlen(oack + str_place)];
    option[strlen(oack + str_place)] = '\0';
    strncpy(option, oack + str_place, strlen(oack + str_place));
    str_place += strlen(oack + str_place) + 1;

    if (str_place > OACK_MAX_SIZE + 4 || strlen(oack + str_place) == 0)
    {
      print_if_verbose("Found option without corresponding value: %s\n", option);
      return 3;
    }

    char value[strlen(oack + str_place)];
    value[strlen(oack + str_place)] = '\0';
    strncpy(value, oack + str_place, strlen(oack + str_place));
    str_place += strlen(oack + str_place) + 1;

    if (strcmp(option, BLOCKSIZE_FLAG) == 0)
    {
      if (!*blocksize_ptr)
      {
        print_if_verbose("Got option in OACK that client did not request: %s\n", option);
        return 2;
      }
      else
      {
        int server_val = (int)strtol(value, (char **)NULL, 10);

        if (server_val != *blocksize_ptr)
        {
          print_if_verbose("NOTE: Server overwrote option: %s from %d to %d\n", option, *blocksize_ptr, server_val);
          *blocksize_ptr = server_val;
        }
      }
    }
    else if (strcmp(option, WINDOWSIZE_FLAG) == 0)
    {
      if (!*windowsize_ptr)
      {
        print_if_verbose("Got option in OACK that client did not request: %s\n", option);
        return 2;
      }
      else
      {
        int server_val = (int)strtol(value, (char **)NULL, 10);

        if (server_val != *windowsize_ptr)
        {
          print_if_verbose("NOTE: Server overwrote option: %s from %d to %d\n", option, *windowsize_ptr, server_val);
          *windowsize_ptr = server_val;
        }
      }
    }
    else
    {
      print_if_verbose("Got unknown option in OACK: %s\n", option);
      return 1;
    }
  }
  return 0;
}

void create_oack_err(char *error, int error_length)
{
  memset(error, 0, error_length);

  error[1] = '\x05'; // Opcode 5
  error[3] = '\x08'; // Error code 8

  strncpy(error + 4, ERROR_CODES[8], strlen(ERROR_CODES[8]));
}