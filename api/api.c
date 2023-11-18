#include <stdio.h>
#include <ulfius.h>
#include <encoder.h>
#include <io.h>
#include <main.h>
#include <string.h>
#include <stdbool.h>

#define PORT 5000
// compile with 
// gcc $(pkg-config --cflags libulfius) -o api api.c ../encoder.c ../io.c ../main.c ../decoder.c ../helpers.c  $(pkg-config --libs libulfius) -I ..


bool validateHexadecimalInstruction(char *hexadecimal) {
  if(strlen(hexadecimal) != 10 || strncmp(hexadecimal, "0x", 2) != 0) {
    return false;
  }
  return true;
}

int callback_options(const struct _u_request *request, struct _u_response *response, void *user_data)
{
  u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
  u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
  u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
  return U_CALLBACK_COMPLETE;
}

int callback(const struct _u_request *request, struct _u_response *response, void *user_data) {
  ulfius_set_string_body_response(response, 200, "Testing");
  return U_CALLBACK_CONTINUE;
}

int decodeCallback(const struct _u_request *request, struct _u_response *response, void *user_data) {
  const char *stringToDecode = u_map_get(request->map_post_body, "hexadecimal");
  char *code = strcpy(malloc(strlen(stringToDecode) + 1), stringToDecode);

  bool isValidCode = validateHexadecimalInstruction(code);
  if(!isValidCode) {
    ulfius_set_string_body_response(response, 422, "Error: Invalid body. Please send an 8 digit hexadecimal string: 0x12345678 that corresponds to a mips instruction code.");
    return U_CALLBACK_CONTINUE;
  }

  FILE *in = getFile("entrada.txt", "w");
  writeLine(in, code);
  closeFile(in);
  
  decode();
  
  FILE *out = getFile("saida.asm", "r");
  char *line;
  char *res = malloc(100);

  while((line = getNextLine(out))) {
    strcat(res, line);
  }
  ulfius_set_string_body_response(response, 200, res);
  return U_CALLBACK_CONTINUE;
}

int encodeCallback(const struct _u_request *request, struct _u_response *response, void *user_data) {
  const char *payload = u_map_get(request->map_post_body, "instruction");
  char *instruction = strcpy(malloc(strlen(payload) + 1), payload);

  FILE *in = getFile("entrada.asm", "w");
  writeLine(in, ".text\n.globl\nmain:");
  writeLine(in, instruction);
  closeFile(in);
  
  encode();

  FILE *out = getFile("saida.txt", "r");
  char *line;
  char *res = getNextLine(out);
  
  ulfius_set_string_body_response(response, 200, res);
  return U_CALLBACK_CONTINUE;
}

int main(void) {
  struct _u_instance instance;
  if(ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  u_map_put(instance.default_headers, "Access-Control-Allow-Origin", "*");

  // endpoint list declaration 
  ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &callback_options, NULL);
  ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback, NULL);
  ulfius_add_endpoint_by_val(&instance, "POST", "/decode", NULL, 0, &decodeCallback, NULL);
  ulfius_add_endpoint_by_val(&instance, "POST", "/encode", NULL, 0, &encodeCallback, NULL);

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

