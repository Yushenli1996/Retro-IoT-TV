#!/bin/sh

COMMAND='python ./Desktop/connection.py'
LOGFILE=restart.txt

writelog()
{
	now=`date`
	echo "$now $*" >> $LOGFILE
}

writelog "Starting"
while true ; do
	sleep 2s
	$COMMAND
	#writelog "Exited with status $?"
	#writelog "Restarting"
done
