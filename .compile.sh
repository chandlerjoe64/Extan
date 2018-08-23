#!/bin/bash

c_file=$1
file_name=${c_file%.c}

gcc "$@" -g -lm -o $file_name".out"
#gcc $c_file -g -Wall -Werror -o $file_name".out"
echo $c_file compiled as: $file_name".out"
