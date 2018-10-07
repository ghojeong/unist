#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <pthread.h>

#define PORT 5000
#define BUF_SIZE 1024
#define SMALL_BUF 256 

void* respond (void* arg);
char *ROOT;

const char* content_type(const char* file);
int getFileSize(FILE* fp);
void send_data(int sock, const char* ct, const char* file_name);
void* request_handler(void* arg);


int main( int argc, char *argv[] ) {
  pthread_t t_id;
  void* thr_ret;
  int sockfd, newsockfd, portno = PORT;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  clilen = sizeof(cli_addr);
  ROOT = getenv("PWD");

  if(argc == 2){
    portno = atoi(argv[1]); 
  }

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    printf("perror_ERROR opening socket");
    perror("ERROR opening socket");
    exit(1);
  }

  // port reusable
  int tr = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1) {
    printf("perror_setsockopt");
    perror("setsockopt");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* TODO : Now bind the host address using bind() call.*/
  if( bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 ){
    printf("bind() error");
    exit(1);
  }

  /* TODO : listen on socket you created */
  if( listen(sockfd, 1024) == -1 ){
    printf("listen() error");
    exit(1);
  }


  printf("Server is running on port %d\n", portno);

  for(;;) {
    /* TODO : accept connection */
    newsockfd = accept( sockfd, (struct sockaddr*) &cli_addr, &clilen );
    if(newsockfd == -1){
      printf("accept() error");
    }

    // TODO : implement processing. there are three ways - single threaded, multi threaded, thread pooled
    if( pthread_create(&t_id, NULL, respond, &newsockfd)!=0 ){
      printf("pthread_create() error\n");
    }
    if( pthread_join(t_id, &thr_ret)!=0 ){
      printf("phtread_join() error");
    }
  }
  return 0;
}

void* respond(void* arg) {
  int sock = *((int*)arg);
  int n;
  char buffer[9999];
  char abs_path[256];

  char method[SMALL_BUF];
  char ct[SMALL_BUF];

  n=0;
  memset( buffer, 0, sizeof(buffer) );
  memset( abs_path, 0, sizeof(abs_path) );
  memset( method, 0, sizeof(method) );
  memset( ct, 0, sizeof(ct) );


  bzero(buffer,9999);
  n = recv(sock,buffer,9999, 0);
  if (n < 0) {
    printf("recv() error\n");
    return NULL;
  } else if (n == 0) {
    printf("Client disconnected unexpectedly\n");
    return NULL;
  } else if ( strstr(buffer, "HTTP/")==NULL ){
    printf("not HTTP/\n");
    return NULL;
  }
  else {
//    char* method = strtok(buffer, " \t");
    // TODO : parse received message from client
    // make proper response and send it to client
    strncpy( method, strtok(buffer, " \t"), sizeof(method) );
    strncpy( abs_path, strtok(NULL, " \t"), sizeof(abs_path) );
    strncpy( ct, content_type(abs_path), sizeof(ct) );
    if( strcmp(method, "GET")!=0 ){
      printf("not GET\n");
    }
    if( strlen(abs_path)<4 ){
      memset( abs_path, 0, sizeof(abs_path) );
      memset( ct, 0, sizeof(ct) );
      strncpy( abs_path, "/index.html", sizeof(abs_path) );
      strncpy( ct, "text/html", sizeof(ct) );
    } 
    send_data( sock, abs_path, ct );
  }


  shutdown(sock, SHUT_RDWR);
  close(sock);

  printf("\n\n");
}


const char* content_type(const char* file){
  char extension[SMALL_BUF];
  char file_name[SMALL_BUF];
  char* extension_;

  memset( extension, 0, sizeof(extension) );
  memset( file_name, 0, sizeof(file_name) );

  strncpy( file_name, file, sizeof(file_name) );
  strtok(file_name, ".");

  extension_ = strtok(NULL, ".");
  if( extension_ == NULL ){
    printf("no extension error\n");
    return "text/plain";
  }

  strncpy( extension, extension_, sizeof(extension) );

  if( !strcmp(extension, "html") || !strcmp(extension, "htm") ){
    return "text/html";
  }else if( !strcmp(extension, "css") ){
    return "text/css";
  }else if( !strcmp(extension, "js") ){
    return "application/javascript";
  }else if( !strcmp(extension, "jpg") || !strcmp(extension, "jpeg") || !strcmp(extension, "jpe") || !strcmp(extension, "jif") || !strcmp(extension, "jfif") ){
    return "image/jpeg";
  }

  return "text/plain";
}


int getFileSize(FILE* fp){
  int size;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}


void send_data(int sock, const char* file_name, const char* ct){
  char protocol[] = "HTTP/1.0 200 OK\r\n";
  char server[] = "Server:Linux Web Server \r\n";
  char file_name_[SMALL_BUF];
  char cnt_len[SMALL_BUF] ;
  char cnt_type[SMALL_BUF];
  char buf[BUF_SIZE];
  FILE* send_file;
  int readCnt;

  memset( file_name_, 0, sizeof(file_name_) );
  memset( cnt_len, 0, sizeof(cnt_len) );
  memset( cnt_type, 0, sizeof(cnt_type) );
  memset( buf, 0, BUF_SIZE );

  strncpy( file_name_, ".", sizeof(file_name_) );
  strncat( file_name_, file_name, sizeof(file_name_) );

  send_file = fopen( file_name_, "rb" );
  if(send_file == NULL ){
    printf("send_file is NULL\n");
    printf("%s\n", file_name_);
    return;
  } 

  snprintf( cnt_len, sizeof(cnt_len), "Content_Length:%d \r\n", getFileSize(send_file) );
  snprintf( cnt_type, sizeof(cnt_type), "Content-Type:%s \r\n\r\n", ct );

  //Sending Header Info
  fflush(send_file);

  send(sock,protocol,strlen(protocol), 0);
  send(sock,server,strlen(server), 0);
  send(sock,cnt_len,strlen(cnt_len), 0);
  send(sock,cnt_type,strlen(cnt_type), 0);

  //Request data trasmission
  fflush(send_file);

  for(;;){
    memset( buf, 0, BUF_SIZE );
    readCnt = fread( (void*)buf, 1, BUF_SIZE, send_file );
    send(sock,buf,readCnt, 0);
    if( readCnt < BUF_SIZE ){
      break;
    }
  }
  printf("%s ", file_name_);
  if( feof(send_file)!=0 ) {
    printf("file send complete\n");
  }else{
    printf("file send fail\n");
  }

  fflush(send_file);
  fclose(send_file);
}


