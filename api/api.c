#include <stdio.h>
#include <ulfius.h>
#include <encoder.h>
#include <io.h>
#include <main.h>
#include <string.h>

#define PORT 8080
// compile with 
// gcc $(pkg-config --cflags libulfius) -o api api.c $(pkg-config --libs libulfius) -I../
int callback(const struct _u_request *request, struct _u_response *response, void *user_data) {
  ulfius_set_string_body_response(response, 200, "Testing");
  return U_CALLBACK_CONTINUE;
}

int decodeCallback(const struct _u_request *request, struct _u_response *response, void *user_data) {
  const char *stringToDecode = u_map_get(request->map_post_body, "hexadecimal");
  char *code = strcpy(malloc(strlen(stringToDecode) + 1), stringToDecode);

  FILE *in = getFile("entrada.txt", "w");
  writeLine(in, code);
  closeFile(in);
  
  decode();
  

  printf("String to decode: %s\n", stringToDecode);
  ulfius_set_string_body_response(response, 200, stringToDecode);
  return U_CALLBACK_CONTINUE;
}

int main(void) {
  struct _u_instance instance;

  if(ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback, NULL);
  ulfius_add_endpoint_by_val(&instance, "POST", "/decode", NULL, 0, &decodeCallback, NULL);

  if(ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", PORT);
    getchar();
  } else {
    fprintf(stderr, "Error starting framework");
  }
  printf("end framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);
  return 0;
}

