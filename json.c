
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "json.h"

char *parse_string_value(char *const string, int *index) {
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
  for (; string[*index] != ',' && isdigit(string[*index]); ++(*index)) {
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

struct json_value_t *json_parse_new(char *const string, int *cursor) {
  struct json_value_t *value = malloc(sizeof(struct json_value_t));
  value->next = NULL;

  char ch = string[*cursor];
  if (ch == '{') {
    // object
  } else if (ch == '[') {
    struct json_value_t *children = NULL;
    // jump over the '['
    ++(*cursor);

    children = json_parse_new(string, cursor);
    value->next = children;
    value->type = json_array;

    struct json_value_t *next = children;
    while (string[*cursor] == ',') {
      // skip the comma
      ++(*cursor);

      struct json_value_t *ch = json_parse_new(string, cursor);
      next->next = ch;
      next = ch;
    }

    if (string[*cursor] != ']') {
      // fail with error
    } 

    value->next = children;
  } else if (ch == '"') {
    value->string_value = parse_string_value(string, cursor);
    value->type = json_string;
  } else if (isdigit(ch)) {
    value->value = parse_number_value(string, cursor);
    value->type = json_number;
  } else if (ch == 't' || ch == 'f') {
    value->boolean_value = parse_boolean_value(string, cursor);
    value->type = json_boolean;
  } else if (ch == 'n') {
    parse_null_value(string, cursor);
    value->type = json_null;
  }

  return value;
}

struct json_value_t *json_parse(char *const string) {
  int index = 0;
  return json_parse_new(string, &index);
}


void json_print(struct json_value_t *root) {
  for (struct json_value_t *value = root; value != NULL; value = value->next) {
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