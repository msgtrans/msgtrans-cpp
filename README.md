# msgtrans-cpp
MsgTrans Framework for C++ , based on libevent.


## Build

### For Linux

- OpenSSL 1.0.2u
    
    https://github.com/openssl/openssl/releases/tag/OpenSSL_1_0_2u

- libevent

```sh
sudo apt-get install libevent-dev
```


- glog v0.4.0 
    
    https://github.com/google/glog

```sh
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
``` 

- ConcurrentQueue
    https://github.com/cameron314/concurrentqueue

```sh
$ cp -r concurrentqueue evpp/3rdparty/
``` 

- evpp master
    
    https://github.com/Qihoo360/evpp

```sh
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
```     

- Protocol Buffers v3.12.4
    
    https://github.com/protocolbuffers/protobuf/releases/tag/v3.12.4

```sh
$ mkdir build
$ cd build
$ cmake ../cmake  
$ sudo make install
```     
  

- MsgTrans-CPP

1. Generate code from proto file

```sh
$ cd proto
$ protoc -I=./ --cpp_out=./ *.proto
$ cp key_exchange.pb.h ../include/ee2e
$ cp key_exchange.pb.cc ../src/ee2e

```

2. Build
```sh
$ mkdir build
$ cd build
$ cmake ..  
$ sudo make install
```   



### For Mac OS

- Enviroment

Edit ~/.bashrc or ~/.bash_profile or ~/.zshrc, and input 

```sh
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/include
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=/usr/local/lib:$LIBRARY_PATH
```

- OpenSSL 1.0.2u
    
    https://github.com/openssl/openssl/releases/tag/OpenSSL_1_0_2u
    
```sh
$ ./Configure darwin64-x86_64-cc  --prefix=/usr/local/
$ make
$ sudo make install
```

- libevent

```sh
$ brew install libevent
```

- glog v0.4.0 
    
    https://github.com/google/glog

```sh
$ brew install glog
``` 

- ConcurrentQueue
    
    https://github.com/cameron314/concurrentqueue

```sh
$ cp -r concurrentqueue evpp/3rdparty/
``` 

- evpp master
    https://github.com/Qihoo360/evpp

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make install
```     

- Protocol Buffers v3.12.4
    
    https://github.com/protocolbuffers/protobuf/releases/tag/v3.12.4

```sh
$ ./configure
$ make
$ make install
```     

- MsgTrans-CPP

1. Generate code from proto file

```sh
$ cd proto
$ protoc -I=./ --cpp_out=./ *.proto
$ cp key_exchange.pb.h ../include/ee2e
$ cp key_exchange.pb.cc ../src/ee2e
```

2. Build

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```  

