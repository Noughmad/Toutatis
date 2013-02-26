#!/bin/bash

MODELS="Task Project Note Event"

qdbuscpp2xml daemon/toutatis.h -o com.noughmad.Toutatis.xml
for model in $MODELS;
do
    OUTPUT="com.noughmad.toutatis.${model}.xml"
    qdbuscpp2xml daemon/${model,,}.h -o $OUTPUT
    xml ed -L -s "/node/interface" -t elem -n property -v "" -i "/node/interface/property[last()]" -t attr -n name -v "id" -i "/node/interface/property[last()]" -t attr -n type -v "s" -i "/node/interface/property[last()]" -t attr -n access -v "read" $OUTPUT
    xml ed -L -s "/node/interface" -t elem -n signal -v "" -i "/node/interface/signal[last()]" -t attr -n name -v "removed" $OUTPUT
    xml ed -L -s "/node/interface" -t elem -n method -v "" -i "/node/interface/method[last()]" -t attr -n name -v "remove" $OUTPUT
done