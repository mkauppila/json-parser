#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "json.h"

// test basic primitives 

void test_parsing_null() {
  printf("Test parsing 'null'\n");
  struct json_value_t *root = json_parse("null");
  assert(root->type == json_null);
}

void test_parsing_true() {
  printf("Test parsing 'true'\n");
  struct json_value_t *root = json_parse("true");
  assert(root->type == json_boolean);
  assert(root->boolean_value == true);
}

void test_parsing_false() {
  printf("Test parsing 'false'\n");
  struct json_value_t *root = json_parse("false");
  assert(root->type == json_boolean);
  assert(root->boolean_value == false);
}

void test_parsing_string() {
  printf("Test parsing 'hello'\n");
  struct json_value_t *root = json_parse("\"hello\"");
  assert(root->type == json_string);
  assert(strcmp(root->string_value, "hello") == 0);
}

void test_parsing_integer() {
  printf("Test parsing '12345'\n");
  struct json_value_t *root = json_parse("12345");
  assert(root->type == json_number);
  assert(root->value == 12345);
}

void test_parsing_integer_array_single_item() {
  printf("Test parsing '[1]'\n");
  struct json_value_t *root = json_parse("[1]");
  assert(root->type == json_array);
  assert(root->next->type == json_number);
  assert(root->next->value == 1);
  assert(root->next->next == NULL);
}

void test_parsing_integer_array_multiple_items() {
  printf("Test parsing '[1,2]'\n");
  struct json_value_t *root = json_parse("[1,2,3]");
  assert(root->type == json_array);
  assert(root->next->type == json_number);
  assert(root->next->value == 1);
  assert(root->next->next->value == 2);
  assert(root->next->next->next->value == 3);
  assert(root->next->next->next->next == NULL);
}

int main(int argc, char *argv[]) {
  test_parsing_null();
  test_parsing_true();
  test_parsing_false();
  test_parsing_string();
  test_parsing_integer();
  test_parsing_integer_array_single_item();
  test_parsing_integer_array_multiple_items();

  printf("Tests OK\n");

  return 0;
}