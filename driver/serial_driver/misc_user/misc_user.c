#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define DEV_PATH "/dev/misc_node"
#define READ_BUF_LEN 10
int main()
{
    char readbuf[READ_BUF_LEN];
    int fd, ret;
    fd = open(DEV_PATH, O_RDWR);
    printf("write 123456\n");
    write(fd, "123456", strlen("123456"));
    printf("set offset to 2\n");
    lseek(fd, 2, SEEK_SET);
    ret = read(fd, readbuf, READ_BUF_LEN - 1);
    readbuf[READ_BUF_LEN - 1] = '\0';
    printf("read back, str=%s\n", readbuf);
    close(fd);
    return 0;
}
