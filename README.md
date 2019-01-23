# dev-fib
Linux kernel module that creates device /dev/fibonacci.  Writing to this device should have no effect, however reading at offset k should return the kth fibonacci number.

## Instructions for bullding
cd dev-fib/
make

### Running after build
sudo insmod ./dev-fib.ko
sudo ./client
