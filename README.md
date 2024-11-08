# WebShare-Connect
A program that transfer of files of any size over the web

## Having trouble getting ZeroMQ and czmq on Linux?
Note for building below.
You can clone the repository to any location. 
Make install will put the files into the correct locations.
## To build ZeroMQ follow the steps:
```
git clone https://github.com/zeromq/libzmq.git
cd libzmq
mkdir build
cd build
cmake ..
make
sudo make install

```
## To build czmq follow the steps:
```
git clone https://github.com/zeromq/czmq.git
cd czmq
mkdir build
cd build
cmake ..
make
sudo make install

```
# Build the project on Linux
```
cmake -B compiled/buildLinux/ . && cmake --build compiled/buildLinux
```