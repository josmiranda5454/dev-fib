#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main () {
    int fd;
    long long sz;

    char buf [1];
    char write_buf[] = "testing writing";
    int offset = 100; // Let's test something bigger than the limit
    int i = 0;
    
    fd = open("/dev/fibonacci", O_RDWR);

    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (i = 0; i <= offset ; i++) {
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to /dev/fibonacci, returned the sequence %lld\n", sz);
    }   

    for (i = 0; i <= offset ; i++)
    {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 1);
        printf("Reading from /dev/fibonacci at offset %d, returned the sequence %lld.\n", i, sz);
    }

    printf("Let's go backwards\n");
    
    for (i = offset; i >= 0 ; i--)
    {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 1);
        printf("Reading from /dev/fibonacci at offset %d, returned the sequence %lld.\n", i, sz);
    }

    close(fd);
    return 0;
}
