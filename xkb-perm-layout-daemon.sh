#!/bin/sh

[ -z "$1" ] && exit 1

while true; do
	"$1"
	sleep 1
done
