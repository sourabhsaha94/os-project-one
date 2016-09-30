#!/bin/bash

output_file="results.out"
c_program="time_measurement_overhead"
c_source=$c_program".c"
java_program="CalculateSDMean"
java_source=$java_program".java"

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program ] ; then
    rm $c_program
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi

gcc $c_source -o $c_program
javac $java_source
nice ./$c_program
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program ] ; then
    rm $c_program
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi
