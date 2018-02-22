#!/bin/sh

docker run -v $(pwd):/project:rw -it calliope/build yotta $1
