#!/bin/bash

from=$1
to=$2
file_for_syslog=$3

cat $from > $to
cat /etc/passwd > $file_for_syslog
