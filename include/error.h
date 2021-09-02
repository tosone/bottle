#include <stdio.h>

#pragma once

typedef int bottle_error_t;

typedef struct error_entry_t {
  bottle_error_t num;
  const char *msg;
} error_entry_t;

#define bottle_ok 0
#define error_invalid_command 1
#define error_not_found 2

static const struct error_entry_t error_entries[] = {
    {bottle_ok, ""},
    {error_invalid_command, "invalid command"},
    {error_not_found, ""},
};

const char *errstr(bottle_error_t err);
