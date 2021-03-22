
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

struct json_value_t *create_json_value() {
  struct json_value_t *value = malloc(sizeof(struct json_value_t));
  value->children = NULL;
  value->next = NULL;
  value->string_value = NULL;
  value->name = NULL;
  value->json_value = NULL;

  return value;
}

void json_free(struct json_value_t *root) {
  if (root->children) {
    json_free(root->children);
  }
  if (root->next) {
    json_free(root->next);
  }
  if (root->json_value) {
    json_free(root->json_value);
  }

  // `string_value` is also used for error messages which
  // are not dynamically allocated
  if (root->string_value && root->type != json_parsing_error) {
    free(root->string_value);
  }
  if (root->name) {
    free(root->name);
  }
  free(root);
}

void parse_white_space(char *const string, int *index) {
  for (char ch = string[*index];
       ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
       ch = string[++(*index)]) {
  }
}

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
  char *key = malloc((len + 1) * sizeof(char));
  memcpy(key, (string + ((*index) - len)), len);
  key[len] = '\0';

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

struct json_value_t *create_parse_error(struct json_value_t *node,
                                        char *message) {
  node->type = json_parsing_error;
  node->string_value = message;
  return node;
}

struct json_value_t *json_parse_internal(char *const string, int *cursor) {
  struct json_value_t *value = create_json_value();
  char *error_message;

  parse_white_space(string, cursor);

  char ch = string[*cursor];
  if (ch == '{') {
    parse_white_space(string, cursor);

    struct json_value_t *children = NULL;
    if (string[*cursor] != '{') {
      // TODO: This can't actually happen!
      return create_parse_error(value, "Missing trailing {");
    } else {
      ++(*cursor);
    }

    parse_white_space(string, cursor);

    value->type = json_object;
    value->next = NULL;

    children = json_parse_internal(string, cursor);
    if (children->type == json_parsing_error) {
      json_free(value);
      return children;
    } else {
      value->children = children;
    }

    parse_white_space(string, cursor);

    struct json_value_t *next = children;
    while (string[*cursor] == ',') {
      // skip the comma
      ++(*cursor);

      parse_white_space(string, cursor);

      struct json_value_t *ch = json_parse_internal(string, cursor);
      next->next = ch;
      next = ch;
    }

    parse_white_space(string, cursor);

    if (string[*cursor] != '}') {
      // So this could be also missing ,
      return create_parse_error(value, "Missing trailing }");
    } else {
      ++(*cursor);
    }
  } else if (ch == '[') {
    // jump over the '['
    ++(*cursor);

    parse_white_space(string, cursor);

    value->type = json_array;
    value->next = NULL;

    struct json_value_t *child = json_parse_internal(string, cursor);
    if (child->type == json_parsing_error) {
      json_free(value);
      return child;
    } else {
      value->children = child;
    }

    parse_white_space(string, cursor);

    struct json_value_t *next = child;
    while (string[*cursor] == ',') {
      // skip the comma
      ++(*cursor);

      parse_white_space(string, cursor);

      struct json_value_t *ch = json_parse_internal(string, cursor);
      next->next = ch;
      next = ch;
    }

    parse_white_space(string, cursor);

    if (string[*cursor] != ']') {
      return create_parse_error(value, "Missing trailing ]");
    } else {
      ++(*cursor);
    }
  } else if (ch == '"') {
    // peek ahead if this is just a string or a key-value pair
    int peekCursor = *cursor + 1;
    bool isKeyValuePair = false;
    // find the next ""
    while (string[peekCursor++] != '"') {
    }
    // eat the whitespace
    parse_white_space(string, &peekCursor);

    if (string[peekCursor++] == ':') {
      isKeyValuePair = true;
    }

    if (isKeyValuePair) {
      parse_white_space(string, cursor);

      value->name = parse_string_value(string, cursor);

      parse_white_space(string, cursor);
      if (string[*cursor] != ':') {
        // This can never happen since it's mandatory for kv pair
        // otherwise it'll be handled as string
        printf("Expecting ':', got '%c'\n", string[*cursor]);
      } else {
        (*cursor)++;
      }

      parse_white_space(string, cursor);

      struct json_value_t *v = json_parse_internal(string, cursor);
      if (v->type == json_string) {
        const int length = strlen(v->string_value);
        value->string_value = malloc(length * sizeof(char));
        strncpy(value->string_value, v->string_value, length);
        json_free(v);
      } else if (v->type == json_number) {
        value->value = v->value;
        json_free(v);
      } else if (v->type == json_boolean) {
        value->boolean_value = v->boolean_value;
        json_free(v);
      } else {
        // default case for objects, arrays and null
        value->json_value = v;
      }
      value->type = v->type;
    } else {
      value->string_value = parse_string_value(string, cursor);
      value->type = json_string;
    }
  } else if (isdigit(ch)) {
    value->value = parse_number_value(string, cursor);
    value->type = json_number;
  } else if (ch == 't' || ch == 'f') {
    value->boolean_value = parse_boolean_value(string, cursor);
    value->type = json_boolean;
  } else if (ch == 'n') {
    parse_null_value(string, cursor);
    value->type = json_null;
  } else {
    return create_parse_error(value, "Invalid input");
  }

  parse_white_space(string, cursor);

  return value;
}

struct json_value_t *json_parse(char *const string) {
  int index = 0;
  return json_parse_internal(string, &index);
}

void json_print(struct json_value_t *root) {
  struct json_value_t *value = root;
  if (root == NULL) {
    printf("root is NULL\n");
    return;
  }

  if (value->type == json_string) {
    printf("print string\n");
    printf("(key:value) = (%s:%s)\n", value->name, value->string_value);
  } else if (value->type == json_number) {
    printf("(key:value) = (%s:%d)\n", value->name, value->value);
  } else if (value->type == json_boolean) {
    printf("(key:value) = (%s:%d)\n", value->name, value->boolean_value);
  } else if (value->type == json_null) {
    printf("(key:value) = (%s:NULL)\n", value->name);
  } else if (value->type == json_array) {
    printf("(key:value) = (%s:ARRAY)\n", value->name);
    if (value->children) {
      printf("array has items");
    }
    for (struct json_value_t *child = value->children; child != NULL;
         child = child->next) {
      json_print(child);
    }
  } else if (value->type == json_object) {
    printf("(key:value) = (%s:OBJECT)\n", value->name);
    for (struct json_value_t *child = value->children; child != NULL;
         child = child->next) {
      json_print(child);
    }
  } else if (value->type == json_parsing_error) {
    printf("(key:value) = (NULL:%s)\n", value->string_value);
  }
}
