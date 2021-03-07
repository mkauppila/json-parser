
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "json.h"

char *parse_key(char *const string, int *index) {
  if (string[*index] == '"') {
  } else {
    printf("invalid character, expected '\"', got '%c'", string[*index]);
  }
  (*index)++;

  int len = 0;
  for (; *index < strlen(string); ++(*index)) {
    if (string[*index] == '"') {
      break;
    }
    len++;
  }
  char *key = malloc(len * sizeof(char)); // is there space for the '\0'? No??
  memcpy(key, (string + ((*index) - len)), len);

  // consume the '"' character at the end
  if (string[*index] == '"') {
    (*index)++;
  }

  return key;
}

char *parse_string_value(char *const string, int *index) {
  // parse key does the exact same thing
  return parse_key(string, index);
}

int strnum(char *s, int n) {
  int x = 0;
  int i = 0;
  while (i < n) {
    x = x * 10 + (s[i] - '0');
    i++;
  }
  return x;
}

int parse_number_value(char *const string, int *index) {
  int len = 0;
  for (; *index < strlen(string) && isdigit(string[*index]); ++(*index)) {
    len++;
  }

  int number = strnum((string + ((*index) - len)), len);
  return number;
}

bool parse_boolean_value(char *const string, int *index) {
  if (string[*index] == 't') {
    (*index) += 4;
    return true;
  } else {
    (*index) += 5;
    return false;
  }
}

void parse_null_value(char *const string, int *index) {
  // Nothing to do except move index forward
  if (string[*index] == 'n') {
    (*index) += 4;
  }
}

struct json_value_t *parse_key_value(char *const string, int *index) {
  struct json_value_t *value = malloc(sizeof(struct json_value_t));
  value->next = NULL;

  (*index)++; 
  value->name = parse_key(string, index);
  // consume the ':' from the middle
  // TODO: verify that it actually is ':'

  (*index)++;
  if (string[*index] == '"') {
    value->string_value = parse_string_value(string, index);
    value->type = json_string;
  } else if (isdigit(string[*index])) {
    value->value = parse_number_value(string, index);
    value->type = json_number;
  } else if (string[*index] == 't' || string[*index] == 'f') {
    value->boolean_value = parse_boolean_value(string, index);
    value->type = json_boolean;
  } else if (string[*index] == 'n') {
    parse_null_value(string, index);
    value->type = json_null;
  }
  // if { -> object
  // if [ -> array

  return value;
}
/*
struct json_kit *json_parse(char *const string, int index) {
  struct json_t *node = malloc(sizeof(struct json_t));
  node->values = NULL;
  node->type = json_object;

  for (int cursor = index; cursor < strlen(string); ++cursor) {
    char ch = string[cursor];
    if (ch == '{') {
      int next = cursor + 1; // might go over strlen(string)

      // parse all the key value pairs
      while (string[next] == '"' || string[next] == ',') {
        // verify ',' and skip it
        if (string[next] == ',') {
          cursor++;
        }

        struct json_value_t *value = parse_key_value(string, &cursor);

        if (node->values) {
          struct json_value_t *ptr = node->values;
          while (ptr->next != NULL) {
            ptr = ptr->next;
          }
          ptr->next = value;
        } else {
          node->values = value;
        }

        next = cursor + 1;
      }

      if (string[cursor] == '}') {
        break;
      }
    } else if (ch == '[') {
      // parse array
    } else {
      // error
    }
  }
  return node;
}
*/

// this parses a single element/value
struct json_value_t *json_parse_new(char *const string, int index) {
  struct json_value_t *value = malloc(sizeof(struct json_value_t));
  value->next = NULL;

  for (int cursor = index; cursor < strlen(string); ++cursor) {
    char ch = string[cursor];
    if (ch == '{') {
      // object
    } else if (ch == '[') {
      printf("parse array\n");
      struct json_value_t *children = NULL;
      children = json_parse_new(string, cursor + 1);
      // if next ] then end array, if , then go and parse next one

      value->next = children;
      value->type = json_array;

      // just break instead of processsingall
      break;
    } else if (ch == '"') {
      value->string_value = parse_string_value(string, &cursor);
      value->type = json_string;
    } else if (isdigit(ch)) {
      printf("parse digit\n");
      value->value = parse_number_value(string, &cursor);
      value->type = json_number;
    } else if (ch == 't' || ch == 'f') {
      value->boolean_value = parse_boolean_value(string, &cursor);
      value->type = json_boolean;
    } else if (ch == 'n') {
      parse_null_value(string, &cursor);
      value->type = json_null;
    }
  }

  return value;
}

struct json_value_t *json_parse(char *const string) {
  return json_parse_new(string, 0);
}


void json_print(struct json_value_t *root) {
  struct json_value_t *value = root;

  for (; value != NULL; value = value->next) {
    if (value->type == json_string) {
      printf("(key:value) = (%s:%s)\n", value->name, value->string_value);
    } else if (value->type == json_number) {
      printf("(key:value) = (%s:%d)\n", value->name, value->value);
    } else if (value->type == json_boolean) {
      printf("(key:value) = (%s:%d)\n", value->name, value->boolean_value);
    } else if (value->type == json_null) {
      printf("(key:value) = (%s:NULL)\n", value->name);
    } else if (value->type == json_array) {
      printf("(key:value) = (%s:ARRAY)\n", value->name);
    }
  }
}

void json_free(struct json_value_t *root) {
  // TODO: implement this properly, all the "nodes"
  // need to be freed
  free(root);
}