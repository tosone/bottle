#include <error.h>

const char *errstr(bottle_error_t err) {
  for (size_t i = 0; i < sizeof(error_entries) / sizeof(error_entry_t); i++) {
    if (error_entries[i].num == err) {
      return error_entries[i].msg;
    }
  }
  return NULL;
}
