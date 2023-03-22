#!/bin/bash

# runclient: fetches parameters for the client and runs it automatically                                                                                                                              
#                                                                                                                                                                                 
# usage:                                                                                                                                                                          
#   runclient dir [player]                                                                                                                                                               
#                                                                                                                                                                                 
# Mark Lekina Rorat, March 2023

# validate command-line parameters
if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "usage: $0 dir [player]"
    exit 1
fi

# parse command-line parameters
dir=$1
player=$2

# fetch host
serverHost=$(ipconfig getifaddr en0)

# fetch port number
serverPort=$(grep -o "ready at port '.*'" server.log | cut -d"'" -f2)

# validate host
if [[ -z $serverHost ]]; then
    echo invalid server host $serverHost - exiting...
    exit 2
fi

# validate port number
if [[ $serverPort != [0-9]* ]]; then
    echo invalid server port $serverPort - exiting...
    exit 3
fi

# launch client
if [[ -n "$player" ]]; then
    $dir/client 2>$player.log $serverHost $serverPort $player
else
    $dir/client 2>spectator.log $serverHost $serverPort
fi
