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
  assert(root->children->type == json_number);
  assert(root->children->value == 1);
  assert(root->children->next == NULL);
}

void test_parsing_integer_array_multiple_items() {
  printf("Test parsing '[1,2,3]'\n");
  struct json_value_t *root = json_parse("[1,2,3]");
  assert(root->type == json_array);
  assert(root->children->type == json_number);
  assert(root->children->value == 1);

  assert(root->children->next->type == json_number);
  assert(root->children->next->value == 2);

  assert(root->children->next->next->type == json_number);
  assert(root->children->next->next->value == 3);
  assert(root->children->next->next->next == NULL);
}

void test_parsing_object_with_string() {
  printf("Test parsing '{\"message\":\"hello world!\"}'\n");
  struct json_value_t *root = json_parse("{\"message\":\"hello world!\"}");

  assert(root->type == json_object);
  assert(root->children != NULL);
  assert(root->children->type == json_string);

  assert(strcmp(root->children->name, "message") == 0);
  assert(strcmp(root->children->string_value, "hello world!") == 0);
}

void test_parsing_object_with_multiple_strings() {
  printf("Test parsing '{\"message\":\"hello world!\",\"foo\":\"bar\"}'\n");
  struct json_value_t *root = json_parse("{\"message\":\"hello world!\",\"foo\":\"bar\"}");

  assert(root->type == json_object);
  assert(root->type == json_object);
  assert(root->children != NULL);

  assert(root->children->type == json_string);
  assert(strcmp(root->children->name, "message") == 0);
  assert(strcmp(root->children->string_value, "hello world!") == 0);

  assert(root->children->next->type == json_string);
  assert(strcmp(root->children->next->name, "foo") == 0);
  assert(strcmp(root->children->next->string_value, "bar") == 0);
}

int main(int argc, char *argv[]) {
  test_parsing_null();
  test_parsing_true();
  test_parsing_false();
  test_parsing_string();
  test_parsing_integer();
  test_parsing_integer_array_single_item();
  test_parsing_integer_array_multiple_items();
  test_parsing_object_with_string();
  test_parsing_object_with_multiple_strings();

  printf("Tests OK\n");

  return 0;
}