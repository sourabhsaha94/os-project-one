#!/bin/bash

output_file="results.out"
c_program="ram_access_latency"
c_source=$c_program".c"

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program ] ; then
    rm $c_program
fi

gcc $c_source -o $c_program
nice ./$c_program

if [ -f $c_program ] ; then
    rm $c_program
fi
