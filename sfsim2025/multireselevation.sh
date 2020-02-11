#!/bin/sh
set -e
if [ -z $1 ] || [ -z $2 ]; then
  echo "Syntax: $0 [input data] [output prefix]" >&2
  exit 1
fi
./scale-elevation $1 /tmp/elevation2.png
./scale-elevation /tmp/elevation2.png /tmp/elevation3.png
./scale-elevation /tmp/elevation3.png /tmp/elevation4.png
./scale-elevation /tmp/elevation4.png /tmp/elevation5.png
./elevationtiles $1 675 4 $2
./elevationtiles /tmp/elevation2.png 675 3 $2
./elevationtiles /tmp/elevation3.png 675 2 $2
./elevationtiles /tmp/elevation4.png 675 1 $2
./elevationtiles /tmp/elevation5.png 675 0 $2
exit 0
