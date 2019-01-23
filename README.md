# dev-fib
Linux kernel module that creates device /dev/fibonacci.  Writing to this device should have no effect, however reading at offset k should return the kth fibonacci number.

## Instructions for bullding
1. ```cd dev-fib/```
2. ```make```

### Running after build
1. ```sudo insmod ./dev-fib.ko```
2. ```sudo ./client```

#### References
1. [Writing a simple device driver](https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os)
2. [Character device drivers](https://linux-kernel-labs.github.io/master/labs/device_drivers.html#open-and-release)
3. [cdev interface](https://lwn.net/Articles/195805/)
4. [Character device files](https://sysplay.in/blog/linux-device-drivers/2013/06/character-device-files-creation-operations/)
