# dev-fib
Linux kernel module that creates device /dev/fibonacci.  Writing to this device should have no effect, however reading at offset k should return the kth fibonacci number.

## Instructions for bullding
1. ```cd dev-fib/```
2. ```make```

### Running after build
1. ```sudo insmod ./dev-fib.ko```
2. ```sudo ./client```
