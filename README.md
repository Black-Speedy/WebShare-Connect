# WebShare-Connect
A program that transfer of files of any size over the web

## Compile WebShare-Connect
Must edit the `CMakeUserPresets.json.example` file and save as
`CMakeUserPresets.json`. Both `user-win` and `user-lin` must
exist or cmake will refuse to compile. If you don't need both
fill the one that is needed and ignore the other one. Currently
the presets will not work for crosscompiling meaning to compile
for linux or windows you must be on that OS. It is possible to
use translation layers or virtual machines to crosscompile.

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
