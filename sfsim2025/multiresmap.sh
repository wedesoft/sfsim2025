#!/bin/sh
set -e
if [ -z $4 ]; then
  echo "Syntax: $0 [input image] [output prefix] [y offset] [x offset]" >&2
  exit 1
fi
# Set MAGICK_TMPDIR to file system location with plenty of space!
./scale-image $1 /tmp/map2.png
./scale-image /tmp/map2.png /tmp/map3.png
./scale-image /tmp/map3.png /tmp/map4.png
./scale-image /tmp/map4.png /tmp/map5.png
./scale-image /tmp/map5.png /tmp/map6.png
./maptiles $1 675 5 $2 $3 $4
./maptiles /tmp/map2.png 675 4 $2 $3 $4
./maptiles /tmp/map3.png 675 3 $2 $3 $4
./maptiles /tmp/map4.png 675 2 $2 $3 $4
./maptiles /tmp/map5.png 675 1 $2 $3 $4
./maptiles /tmp/map6.png 675 0 $2 $3 $4
exit 0
