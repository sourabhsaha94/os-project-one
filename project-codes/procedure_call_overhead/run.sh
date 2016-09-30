#!/bin/bash

output_file="results.out"
c_program="procedure_call_overhead"
c_source=$c_program".c"
java_program="CalculateSDMean"
java_source=$java_program".java"

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi

javac $java_source

if [ -f $c_program"0" ] ; then
    rm $c_program"0"
fi

gcc $c_program"0.c" -o $c_program"0"
nice ./$c_program"0"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"0" ] ; then
    rm $c_program"0"
fi

if [ -f $c_program"1" ] ; then
    rm $c_program"1"
fi

gcc $c_program"1.c" -o $c_program"1"
nice ./$c_program"1"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"1" ] ; then
    rm $c_program"1"
fi

if [ -f $c_program"2" ] ; then
    rm $c_program"2"
fi

gcc $c_program"2.c" -o $c_program"2"
nice ./$c_program"2"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"2" ] ; then
    rm $c_program"2"
fi

if [ -f $c_program"3" ] ; then
    rm $c_program"3"
fi

gcc $c_program"3.c" -o $c_program"3"
nice ./$c_program"3"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"3" ] ; then
    rm $c_program"3"
fi

if [ -f $c_program"4" ] ; then
    rm $c_program"4"
fi

gcc $c_program"4.c" -o $c_program"4"
nice ./$c_program"4"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"4" ] ; then
    rm $c_program"4"
fi

if [ -f $c_program"5" ] ; then
    rm $c_program"5"
fi

gcc $c_program"5.c" -o $c_program"5"
nice ./$c_program"5"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"5" ] ; then
    rm $c_program"5"
fi

if [ -f $c_program"6" ] ; then
    rm $c_program"6"
fi

gcc $c_program"6.c" -o $c_program"6"
nice ./$c_program"6"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"6" ] ; then
    rm $c_program"6"
fi

if [ -f $c_program"7" ] ; then
    rm $c_program"7"
fi

gcc $c_program"7.c" -o $c_program"7"
nice ./$c_program"7"
java $java_program

if [ -f $output_file ] ; then
    rm $output_file
fi

if [ -f $c_program"7" ] ; then
    rm $c_program"7"
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi
