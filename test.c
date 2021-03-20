#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "json.h"

// test basic primitives

void test_parsing_null() {
  printf("Test parsing 'null'\n");
  struct json_value_t *root = json_parse("null");
  assert(root->type == json_null);

  json_free(root);
}

void test_parsing_true() {
  printf("Test parsing 'true'\n");
  struct json_value_t *root = json_parse("true");
  assert(root->type == json_boolean);
  assert(root->boolean_value == true);

  json_free(root);
}

void test_parsing_false() {
  printf("Test parsing 'false'\n");
  struct json_value_t *root = json_parse("false");
  assert(root->type == json_boolean);
  assert(root->boolean_value == false);

  json_free(root);
}

void test_parsing_string() {
  printf("Test parsing 'hello'\n");
  struct json_value_t *root = json_parse("\"hello\"");
  assert(root->type == json_string);
  assert(strcmp(root->string_value, "hello") == 0);

  json_free(root);
}

void test_parsing_integer() {
  printf("Test parsing '12345'\n");
  struct json_value_t *root = json_parse("12345");
  assert(root->type == json_number);
  assert(root->value == 12345);

  json_free(root);
}

void test_parsing_integer_array_single_item() {
  printf("Test parsing '[1]'\n");
  struct json_value_t *root = json_parse("[1]");
  assert(root->type == json_array);
  assert(root->children->type == json_number);
  assert(root->children->value == 1);
  assert(root->children->next == NULL);

  json_free(root);
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

  json_free(root);
}

void test_parsing_object_with_string() {
  printf("Test parsing '{\"message\":\"hello world!\"}'\n");
  struct json_value_t *root = json_parse("{\"message\":\"hello world!\"}");

  assert(root->type == json_object);
  assert(root->children != NULL);
  assert(root->children->type == json_string);

  assert(strcmp(root->children->name, "message") == 0);
  assert(strcmp(root->children->string_value, "hello world!") == 0);

  json_free(root);
}

void test_parsing_object_with_multiple_strings() {
  printf("Test parsing '{\"message\":\"hello world!\",\"foo\":\"bar\"}'\n");
  struct json_value_t *root =
      json_parse("{\"message\":\"hello world!\",\"foo\":\"bar\"}");

  assert(root->type == json_object);
  assert(root->children != NULL);

  assert(root->children->type == json_string);
  assert(strcmp(root->children->name, "message") == 0);
  assert(strcmp(root->children->string_value, "hello world!") == 0);

  assert(root->children->next->type == json_string);
  assert(strcmp(root->children->next->name, "foo") == 0);
  assert(strcmp(root->children->next->string_value, "bar") == 0);

  json_free(root);
}

void mem_leak_test() {
  printf("mem leak");
  struct json_value_t *root = json_parse("{\"null\":null}");
  json_free(root);
}

void test_parsing_object_with_multiple_basic_values() {
  printf(
      "Test parsing "
      "'{\"foo\":\"bar\",\"number\":12345,\"true\":true,\"false\":false}'\n");

  struct json_value_t *root =
      json_parse("{\"foo\":\"bar\",\"number\":12345,\"true\":true,\"false\":"
                 "false,\"null\":null}");

  assert(root->type == json_object);
  assert(root->children != NULL);

  assert(root->children->type == json_string);
  assert(strcmp(root->children->name, "foo") == 0);
  assert(strcmp(root->children->string_value, "bar") == 0);

  assert(root->children->next->type == json_number);
  assert(strcmp(root->children->next->name, "number") == 0);
  assert(root->children->next->value == 12345);

  assert(root->children->next->next->type == json_boolean);
  assert(strcmp(root->children->next->next->name, "true") == 0);
  assert(root->children->next->next->boolean_value == true);

  assert(root->children->next->next->next->type == json_boolean);
  assert(strcmp(root->children->next->next->next->name, "false") == 0);
  assert(root->children->next->next->next->boolean_value == false);

  assert(root->children->next->next->next->next->type == json_null);
  assert(strcmp(root->children->next->next->next->next->name, "null") == 0);

  json_free(root);
}

void test_parsing_object_with_object_and_array() {
  printf("Test parsing '{\"foo\":{\"bar\":1},\"arr\":[1,\"hello\"]}'\n");

  struct json_value_t *root =
      json_parse("{\"foo\":{\"bar\":1},\"arr\":[1,\"hello\"]}");

  assert(root->type == json_object);
  assert(root->children != NULL);

  assert(root->children->type == json_object);
  assert(strcmp(root->children->name, "foo") == 0);
  assert(root->children->json_value != NULL);
  assert(root->children->json_value->type == json_object);

  assert(root->children->json_value->children->type == json_number);
  assert(strcmp(root->children->json_value->children->name, "bar") == 0);
  assert(root->children->json_value->children->value == 1);

  assert(strcmp(root->children->next->name, "arr") == 0);
  assert(root->children->next->json_value->type == json_array);

  assert(root->children->next->json_value->children->value = 1);
  assert(strcmp(root->children->next->json_value->children->next->string_value,
                "hello") == 0);

  json_free(root);
}

void test_parsing_null_with_whitespace() {
  printf("Test parsing 'null' with whitespace\n");
  struct json_value_t *root = json_parse(" \t \n \r null  ");
  assert(root->type == json_null);

  json_free(root);
}

void test_parsing_object_with_whitespace() {
  printf("Test parsing ...\n");

  struct json_value_t *root = json_parse(
      "\t { \"foo\": \"bar\",\n\t\"number\" : 12345,\"true\" "
      ":\ttrue,\"false\"\t\r:false,\"null\" : null,\r \"arr\": [1, 2,3]}");

  assert(root->type == json_object);
  assert(root->children != NULL);

  assert(root->children->type == json_string);
  assert(strcmp(root->children->name, "foo") == 0);
  assert(strcmp(root->children->string_value, "bar") == 0);

  assert(root->children->next->type == json_number);
  assert(strcmp(root->children->next->name, "number") == 0);
  assert(root->children->next->value == 12345);

  assert(root->children->next->next->type == json_boolean);
  assert(strcmp(root->children->next->next->name, "true") == 0);
  assert(root->children->next->next->boolean_value == true);

  assert(root->children->next->next->next->type == json_boolean);
  assert(strcmp(root->children->next->next->next->name, "false") == 0);
  assert(root->children->next->next->next->boolean_value == false);

  assert(root->children->next->next->next->next->type == json_null);
  assert(strcmp(root->children->next->next->next->next->name, "null") == 0);

  assert(root->children->next->next->next->next->next->type == json_array);
  assert(strcmp(root->children->next->next->next->next->next->name, "arr") ==
         0);

  struct json_value_t *array = root->children->next->next->next->next->next;
  // TODO: somewhoe this json_value is really confusing, shouldn't this just be
  assert(array->json_value->children->value == 1);
  assert(array->json_value->children->next->value == 2);
  assert(array->json_value->children->next->next->value == 3);

  json_free(root);
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
  test_parsing_object_with_multiple_basic_values();
  test_parsing_object_with_object_and_array();
  test_parsing_null_with_whitespace();
  test_parsing_object_with_whitespace();

  mem_leak_test();

  printf("Tests OK\n");

  return 0;
}