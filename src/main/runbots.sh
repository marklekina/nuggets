#!/bin/bash
 
if [[ $# != 3 ]]; then
    echo usage: $0 dir hostname port
    exit 1
fi

workingDir=$1
serverHost=$2
serverPort=$3
 
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