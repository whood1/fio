#include "fio_cli.h"
#include "main.h"
#include <stdio.h>

void test() {
  printf("test worked\n");
  
  return;
}

/* TODO: edit this function to handle HTTP data and answer Websocket requests.*/
static void on_http_request(http_s *h) {
  /* set a response and send it (finnish vs. destroy). */

  //FIOBJ path = fiobj_hash_get(h->path, HTTP_HEADER_HOST);
  //fio_str_info_s path_buf= fiobj_obj2cstr(path);
  //printf("\n%s\n", path_buf.data);
  
  //open file
  FILE *fp;
  char buffer[512];
  int chars = 0;
  //char current_char;
  
  fp = fopen("../assets/index.html", "r");
  if (!fp) { printf("Error\n"); }
  else {

    printf("Success\n");

    for (int i = 0; i < 512; ++i) {
      ++chars;
      buffer[i] = getc(fp);
      if (buffer[i] == EOF) {break;}
      //printf("%c", buffer[i]);
    }
    
  }

  
  //  printf("%d", strlen);
  char* html = (char *)malloc(strlen(buffer) + 1);
  //char html[512];
  strcpy(html, buffer);

  //  printf("%s\n", buffer);
 
  
  int size = sizeof(html);
  size = chars - 1;
  printf("\n\n>>HTML size %d<<\n\n%s\n", chars, html);

  
  //sets content type HTML
  http_set_header(h, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find("html", 4));

  //sets header with Data:data info, just testing
  http_set_header(h, fiobj_str_new("Test", 4), fiobj_str_new("Test", 4));
  
  http_send_body(h, buffer, size);

  free(html);
}

/* starts a listeninng socket for HTTP connections. */
void initialize_http_service(void) {
  /* listen for inncoming connections */
  if (http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
                  .on_request = on_http_request,
                  .max_body_size = fio_cli_get_i("-maxbd") * 1024 * 1024,
                  .ws_max_msg_size = fio_cli_get_i("-max-msg") * 1024,
                  .public_folder = fio_cli_get("-public"),
                  .log = fio_cli_get_bool("-log"),
                  .timeout = fio_cli_get_i("-keep-alive"),
                  .ws_timeout = fio_cli_get_i("-ping")) == -1) {
    /* listen failed ?*/
    perror("ERROR: facil couldn't initialize HTTP service (already running?)");
    exit(1);
  }
}
