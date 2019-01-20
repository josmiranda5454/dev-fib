#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main () {
    int fd;
    long long sz;

    char buf [1];
    char write_buf[] = "testing writing";
    int offset = 89;
    
    fd = open("/dev/fibonacci", O_RDWR);

    lseek(fd, offset, SEEK_SET);
    sz = read(fd, buf, 1);

    printf("Reading from /dev/fibonacci at offset %d, returned the sequence %lld.\n", offset, sz);

    sz = write(fd, write_buf, strlen(write_buf));
    printf("Writing to /dev/fibonacci, returned the sequence %lld\n", sz);
    
    sz = read(fd, buf, 1);

    printf("Reading from /dev/fibonacci at offset %d, returned the sequence %lld.\n", offset, sz);
    
    close(fd);
    
    return 0;
}
