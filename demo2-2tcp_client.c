#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

int tcp_fd;

void *func(void *arg)
{
	// 通信
	char buf[1024] = {0};
	while (1)
	{
		memset(buf, 0, 1024);
		read(tcp_fd, buf, 1024);
		printf("当前获取的数据=%s\n", buf);
		if (strcmp(buf, "bye") == 0)
			break;
	}
}

int main(int argc, char const *argv[])
{
	pthread_t tid;
	pthread_create(&tid, NULL, func, NULL);

	// 1.创建套接字      // ipv4   tcp协议   扩展协议
	tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_fd == -1)
	{
		perror("socket error\n");
		return -1;
	}
	// 2.准备服务端的地址数据
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;	 // ipv4
	server_addr.sin_port = htons(50000); // 该端口为服务器端口
	// 服务端代码运行的设备的ip地址
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	// 3.发起连接请求
	printf("发起连接请求\n");
	int ret = connect(tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret == -1)
	{
		perror("连接失败\n");
		return -1;
	}
	printf("连接成功\n");
	// 4.发送数据
	char buf[1024] = {0};
	while (1)
	{
		memset(buf, 0, 1024);
		printf("请输入准备发送的数据\n");
		scanf("%s", buf);
		write(tcp_fd, buf, strlen(buf));
		if (strcmp(buf, "bye") == 0)
			break;
	}
	pthread_join(tid, NULL);
	// 关闭套接字
	close(tcp_fd);

	return 0;
}
