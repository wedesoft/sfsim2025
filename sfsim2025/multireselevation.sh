#!/bin/sh
set -e
if [ -z $4 ]; then
  echo "Syntax: $0 [input data] [output prefix] [y offset] [x offset]" >&2
  exit 1
fi
./scale-elevation $1 /tmp/elevation2.raw
./scale-elevation /tmp/elevation2.raw /tmp/elevation3.raw
./scale-elevation /tmp/elevation3.raw /tmp/elevation4.raw
./scale-elevation /tmp/elevation4.raw /tmp/elevation5.raw
./elevationtiles $1 675 4 $2 $3 $4
./elevationtiles /tmp/elevation2.raw 675 3 $2 $3 $4
./elevationtiles /tmp/elevation3.raw 675 2 $2 $3 $4
./elevationtiles /tmp/elevation4.raw 675 1 $2 $3 $4
./elevationtiles /tmp/elevation5.raw 675 0 $2 $3 $4
exit 0
