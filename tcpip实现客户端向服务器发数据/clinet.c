#include "net.h"

int main(void)
{
  int fd = -1;
  struct sockaddr_in sin;

  /*创建socket fd*/
  if((fd = socket(AF_INET, SOCK_STREAM, 0))<0)
  {
	perror("socket err");
	exit(1);
  }
  /*连接服务器*/
  /**2、1填充struct sockaddr_in结构体变量*/
  bzero(&sin , sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERV_PORT);	//将本地字节序转换为网络

#if 0
	 sin.sin_addr.s_addr = htonl(INADDY_ANY);
#else
	if( inet_pton(AF_INET, SERV_IP_ADDR, (void *)&sin.sin_addr.s_addr)!=1)
	{
		   perror("inet_pton");
		   exit(1);
	}
#endif
/*
#if 0
	 sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);  // 简单的
#else
	if( inet_pton(AF_INET, SERV_IP_ADDR, (void *)&sin.sin_addr.s_addr)!=1)
	{
		   perror("inet_pton");
		   exit(1);
	}
#endif
*/

  if(connect(fd , (struct sockaddr *)&sin, sizeof(sin))<0)
  {
	  perror("connect error");
	  exit(1);
  }
	
  /*读写数据,写数据*/
  //从键盘上获取数据
    char buf[BUFSIZE];
    while(1){
      bzero(buf,BUFSIZE);
      if(fgets(buf,BUFSIZE-1,stdin) ==NULL){
        continue;
      }
      write(fd,buf,strlen(buf));
  		  if( !strncasecmp(buf, QUIT_STR, strlen(QUIT_STR))){   //用户输入了quit字符  
            printf("client is exiting!\n");
            break;
         }
   }

  /*关闭套接字*/
  close(fd);




}