# WebShare-Connect
A program that transfer of files of any size over the web

## Trouble with connecting?
The one sending files will need to allow connections from the port/program
Ensure that you use the right ip-address. You can also you dns domains if they are linked to the correct ip-address
### Still trouble?
Then you might be behind a NAT. In this case the easiet fix is to use a VPN or similar that allows port forwarding. Some examples:
1. Proton(Paid Version)
2. Tailscape
3. Wireguard

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
If you are inside the outmost **WebShare-Connect/** directory
```
git clone https://github.com/zeromq/czmq.git
cd czmq
mkdir build
cd build
cmake ..
make
sudo make install
```
## Build the project using CLI
```
cmake -B compiled/build/ . && cmake --build compiled/build
```

## Usage
There are two modes of operation.
1. The first mode is where you give the input when calling the compiled program such as:
    1. Linux:
        ```
        ./WebShare-Connect <operation_mode> <port> <threads> <filepath>
        ```
    2. On Windows:
        ```
        ./WebShare-Connect.exe <operation_mode> <port> <threads> <filepath>
        ```

2. The second mode is where you open the program and it guides you through the input.

The two modes of operation modes that are supported are <em>sender</em> / <em>server</em> and <em>receiver</em> / <em>client</em>
