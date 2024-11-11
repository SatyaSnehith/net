#!/bin/bash
output=`echo ${1%}.c`
gcc $output handle_socket.c -pthread -o $1
./`echo $1`
