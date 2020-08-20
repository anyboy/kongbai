#include "net.h"
int main(void)
{
  int fd = -1;
  int newfd = -1;
  struct sockaddr_in sin;

  /*创建socket fd*/
  if((fd = socket(AF_INET, SOCK_STREAM, 0))<0)
  {
	perror("socket err");
	exit(1);
  }
  
  /**2、绑定*/
  /**2、1填充struct sockaddr_in结构体变量*/
  bzero(&sin , sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERV_PORT);	//将本地字节序转换为网络
  
	/**优化1、让服务器程序能绑定在任意的IP上*/
 
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



  /**2、2绑定*/ 
  if((bind(fd , (struct sockaddr *)&sin, sizeof(sin)))<0)
  {
	  perror("bind error");
	  exit(1);
  }
   /**3、调用 listen()把主动套接字变为被动套接字*/
   if((listen(fd, BACKLOG))<0){
      perror("listen");
      exit(1);
  }
   /**4、阻塞等待客户端的连接请求*/
#if 0
	   newfd = accept(fd, NULL, NULL);
	   if(newfd <0)
	   {
	       perror("accept");
	       exit(1);
	   }
#else
      struct sockaddr_in cin;
      socklen_t addrlen = sizeof(cin);
      if((newfd=accept(fd, &cin, &addrlen))<0){
  		perror("accept");
  		exit(1);
	    char ipv4_addr[16];
  		if(!inet_ntop(AF_INET,(void *)&cin.sin_addr,ipv4_addr,sizeof(cin)))
  		{
			perror("inet_ntop");
			exit(1);
		}
		printf("Clinet(%s:%d) is connected!\n",ipv4_addr,ntons(cin.sin_port));
	  }
#endif

  /**5、读写*/
  //和newfd进行数据读写
  int ret = -1;
  char buf[BUFSIZE];
  while(1){
		  bzero(buf, BUFSIZE);
		  do{
				 ret =read(newfd, buf, BUFSIZE-1);
		  }while(ret < 0 &&EINTR == errno );

		  if(ret < 0){
			 perror("read error");
			 exit(1);
		  }
		  
		  if(0 ==ret){	//对方已经关闭
				 break;
		  }
		  

		  printf("Receive data : %s\n",buf);
		  if( !strncasecmp(buf, QUIT_STR, strlen(QUIT_STR))){   //用户输入了quit字符  
		     printf("client is exiting!\n");
		     break;
		  }


  	}


  close (newfd);
  close (fd);

  
  return 0;
}


