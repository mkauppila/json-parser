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
} json_type_t;

struct json_value_t {
  struct json_value_t *next;
  json_type_t type;

  char *name; // "name" of the property AKA key

  int value;           // numeric value. int, decimal, boolean?
  bool boolean_value;  // for boolean
  char *string_value;  // string value
  struct json_t *json; // object or array
};

struct json_t {
  json_type_t type;
  struct json_value_t *values; 
};

void json_print(struct json_t *root);
void json_free(struct json_t *root);

struct json_value_t *json_parse(char *const string);
struct json_value_t *json_parse_new(char *const string, int index);

#endif 