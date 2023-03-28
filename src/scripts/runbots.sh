#!/bin/bash

# runbots: run the nuggets game with a bunch of bots                                                                                                                              
#                                                                                                                                                                                 
# usage:                                                                                                                                                                          
#   ./runbots dir                                                                                                                                                    
#                                                                                                                                                                                 
# David Kotz, May 2019; Mark Lekina Rorat, March 2023

# validate command line parameters
if [[ $# != 1 ]]; then
    echo usage: $0 dir
    exit 1
fi

workingDir=$1

# fetch host
serverHost=$(ipconfig getifaddr en0)

# fetch port number
serverPort=$(grep -o "ready at port '.*'" $workingDir/server.log | cut -d"'" -f2)
 
function cleanup {
    # kill any wayward child processes
    pkill -P $$
}
 
# kill child processes if this script is terminated
trap cleanup SIGTERM EXIT
 
if [[ "$serverPort" != [0-9]* ]]; then
    echo invalid server port "$serverPort" - exiting...
    exit 2
fi
 
echo start the player bots...
for let in {A..Z}; do
    $workingDir/client &>player$let.log $serverHost $serverPort botbg &
done
 
echo start the spectator...
$workingDir/client 2>spectator.log $serverHost $serverPort
 
echo wait for all to finish...
sleep 1
cleanup
exit 0