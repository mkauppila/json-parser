#ifndef JSON_H
#define JSON_H

#include <stdbool.h>

typedef enum {
  json_number = 0,
  json_string,
  json_boolean,
  json_null,
  json_object,
  json_array,
  json_parsing_error,
} json_type_t;

struct json_value_t {
  struct json_value_t *next;
  struct json_value_t *children;
  json_type_t type;

  char *name; // "name" of the property AKA key

  int value;                       // for int
  bool boolean_value;              // for boolean
  char *string_value;              // string value
  struct json_value_t *json_value; // object or array
};

struct json_value_t *json_parse(char *const string);
void json_print(struct json_value_t *root);
void json_free(struct json_value_t *root);

#endif