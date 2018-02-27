#!/bin/sh

docker run -v $(pwd):/project:rw -it calliopeedu/yotta yotta $1
