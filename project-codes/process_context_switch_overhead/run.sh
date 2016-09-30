#!/bin/bash
#RUN THIS FILE WITH SUPERUSER

start_output_file="start_results.out"
end_output_file="end_results.out"
c_program="process_context_switch_overhead"
c_source=$c_program".c"
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

if [ -f $c_program ] ; then
    rm $c_program
fi

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi

gcc $c_source -o $c_program
javac $java_source
for count in {1..20000}
do
  nice ./$c_program
done
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

if [ -f $java_program".class" ] ; then
    rm $java_program".class"
fi
