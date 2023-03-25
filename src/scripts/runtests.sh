#!/bin/bash

# runtests: runs unit tests for the server modules                                                                                                                            
#                                                                                                                                                                                 
# usage:                                                                                                                                                                          
#   ./runtests dir                                                                                                                                                            
#                                                                                                                                                                                 
# Mark Lekina Rorat, March 2023

# ensure tests directory is passed as a parameter
if [[ $# != 1 ]]; then
    echo usage: $0 dir
    exit 1
fi
testsDir=$1

# run unit tests
$testsDir/pointtest
$testsDir/piletest
$testsDir/playertest
$testsDir/gridtest
$testsDir/gametest
