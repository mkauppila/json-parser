#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
  json_number = 0,
  json_string,
  json_boolean,
  json_null,
  json_object,
} json_type_t;

struct json_t;
void json_print(struct json_t *root);

struct json_value_t {
  struct json_value_t *next;
  json_type_t type;

  char *name; // "name" of the property AKA key

  int value;           // numeric value. int, decimal, boolean?
  bool boolean_value;   // for boolean
  char *string_value;  // string value
  struct json_t *json; // object or array
};

struct json_t {
  json_type_t type; // either object or array? Needs this because printing?
  struct json_value_t *values; // all values ({"hello", "world"}()
};

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

void *parse_null_value(char *const string, int *index) {
  if (string[*index] == 'n') {
    (*index) += 4;
    return NULL; // what to return here adnd what to do with this?
  }
  return NULL; // FIXME also what to do here?
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
    value->string_value = parse_null_value(string, index);
    value->type = json_null;
  }
  // if { -> object
  // if [ -> array

  return value;
}

struct json_t *json_parse(char *const string, int index) {
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

void json_print(struct json_t *root) {
  struct json_value_t *value = root->values;
  for (; value != NULL; value = value->next) {
    if (value->type == json_string) {
      printf("(key:value) = (%s:%s)\n", value->name, value->string_value);
    } else if (value->type == json_number) {
      printf("(key:value) = (%s:%d)\n", value->name, value->value);
    } else if (value->type == json_boolean) {
      printf("(key:value) = (%s:%d)\n", value->name, value->boolean_value);
    } else if (value->type == json_null) {
      printf("(key:value) = (%s:NULL)\n", value->name);
    }
  }
}

void json_free(struct json_t *root) {
  // TODO: implement this properly, all the "nodes"
  // need to be freed
  free(root);
}

int main(int argc, char *argv[]) {
  struct json_t *json = json_parse("{\"msg-hello\":12345,\"a\":null,\"isEnabled\":false,\"foo\":\"bar\"}", 0);
  printf("parsing done succesfully!\n");
  json_print(json);

  json_free(json);

  return 0;
}
