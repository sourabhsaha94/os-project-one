#!/bin/bash

c_program="ram_io_write_latency"
c_source=$c_program".c"

if [ -f $c_program ] ; then
    rm $c_program
fi

gcc $c_source -o $c_program
nice ./$c_program

if [ -f $c_program ] ; then
    rm $c_program
fi
