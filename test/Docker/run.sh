#!/bin/sh

script_dir="`cd $(dirname $0); pwd`"

docker run \
  --rm \
  -v $script_dir/../..:/usr/src/GenericLabelInterpolator \
    insighttoolkit/genericlabelinterpolator-test \
      /usr/src/GenericLabelInterpolator/test/Docker/test.sh
