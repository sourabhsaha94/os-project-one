#!/bin/bash

start_output_file="start_results.out"
end_output_file="end_results.out"
c_program="process_creation_overhead"
c_source=$c_program".c"
c_child_program="process_to_be_created"
c_child_source=$c_child_program".c"
java_program="CalculateSDMean"
java_source=$java_program".java"
output_file="results.out"

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $end_output_file ] ; then
    rm $end_output_file
fi

if [ -f $start_output_file ] ; then
    rm $start_output_file
fi

if [ -f $c_child_program ] ; then
    rm $c_child_program
fi

if [ -f $c_program ] ; then
    rm $c_program
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi

gcc $c_source -o $c_program
gcc $c_child_source -o $c_child_program
javac $java_source
nice ./$c_program
java $java_program

if [ -f $start_output_file ] ; then
    rm $start_output_file
fi

if [ -f $end_output_file ] ; then
    rm $end_output_file
fi

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program ] ; then
    rm $c_program
fi

if [ -f $c_child_program ] ; then
    rm $c_child_program
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi
