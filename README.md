# WiFiEvent Socket Library

opensocket is c++ library for socket programming

## Dependencies
* **glog:** goole logging library (**GitHub:** [google/glog](https://github.com/google/glog))
* **openssl:** crypto transfer library (**GitHub:** [openssl/openssl](https://github.com/openssl/openssl))
* **cmake:** c++ build

## Build
0. Install dependencies library
        
        sudo apt install libssl-dev libgoogle-glog-dev cmake g++

1. Get source code this repository. clone with git

        git clone https://github.com/wifievent/opensocket.git
        cd opensocket

2. Cmake build
        
        cd build
        cmake .
        make

## Examples
Library use examples:
[Examples for UDP](app/example)
[Examples for TCP/SSL(TLS)](https://github.com/maro5397/tlstimer)