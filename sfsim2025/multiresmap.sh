#!/bin/sh
set -e
if [ -z $1 ] || [ -z $2]; then
  echo "Syntax: $0 [input image] [output prefix]" >&2
  exit 1
fi
./scale50 $1 /tmp/map2.png
./scale50 /tmp/map2.png /tmp/map3.png
./scale50 /tmp/map3.png /tmp/map4.png
./scale50 /tmp/map4.png /tmp/map5.png
./scale50 /tmp/map5.png /tmp/map6.png
./maptiles $1 675 5 $2
./maptiles /tmp/map2.png 675 4 $2
./maptiles /tmp/map3.png 675 3 $2
./maptiles /tmp/map4.png 675 2 $2
./maptiles /tmp/map5.png 675 1 $2
./maptiles /tmp/map6.png 675 0 $2
exit 0
