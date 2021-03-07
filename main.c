#include <stdio.h>
#include "json.h"


// Start doing this in for all objecrts recursively
// null -> is valid, produceds th right thing
// "aa" -> is valid and string
// so on
// until the current object
// then object with objects

// how to tets errors?

int main(int argc, char *argv[]) {
  struct json_t *json = json_parse("{\"msg-hello\":12345,\"a\":null,\"isEnabled\":false,\"foo\":\"bar\"}", 0);
  printf("parsing done succesfully!\n");
  json_print(json);

  json_free(json);

  return 0;
}
