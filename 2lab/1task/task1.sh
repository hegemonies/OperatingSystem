#!/bin/bash

#1
ps -do pid,uid
ps -u
pstree -p
ps -eo pid,tty,ppid,command,s | grep Z  && kill ****