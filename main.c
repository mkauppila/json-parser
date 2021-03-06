#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct json_t;

struct json_value_t { 
  struct json_value_t *next; // it's a linked list
  int type;

  char *name; // "name" of the property 
  
  double value; // numeric value. int, decimal, boolean?
  char  *stringValue; // string value
  struct json_t *json;// object or array
};


struct json_t {
  int type; // either object or array? Needs this because 
  struct json_value_t *values; // all values ({"hello", "world"}()
};

*struct json_value_t *parse_key(char *const string, int index) {
  struct json_value_t *value = malloc(sizeof(struct json_value_t));

  if (string[index] == '"') {
  } else {
    // excepted " character
  }
  index++;
  
  int len = 0;
  for (; index < strlen(string); ++index) { 
    if (string[index] == '"') {
      break;
    }
    len++;
  }
  printf("key has lenght %d\n", len);
  value->name = malloc(len * sizeof(char)); // is there space for the '\0'? No??
  memcpy(value->name, (string + (index - len)), len);
  printf("key key %s\n", value->name);

  // consume the '"' character at the end
  index++;
  // TODO: should also update the index accordingly, probably needs to be as pointer

  return value;
}

struct json_t *json_parse(char *const string, int index) {
  struct json_t *node = malloc(sizeof(struct json_t));

  for (int cursor = index; cursor < strlen(string); ++cursor) {
    char ch = string[cursor];
    // printf("%c\n", ch);
    if (ch == '{') {
      struct json_value_t *value = parse_key(string, cursor + 1);
      // consume the ':' from the middle
      // parse the value (in this case another string)
        // if " -> string
        // if 0-9 -> number
        // if { -> object
        // if [ -> array

      // parse object until the next } on this level
    } else if (ch == '[') {
      // parse array
    } else {
      // error
    }
  }
  return node;
}

void json_free(struct json_t *root) {
  // TODO: implement this properly, all the "nodes" 
  // need to be freed
  free(root);
}


int main(int argc, char *argv[]) {
  struct json_t *json = json_parse("{\"msg-hello\":\"world world\"", 0);
  // do someting

  // write custon
  json_free(json);

  return 0;
}
