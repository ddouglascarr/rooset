#!/bin/bash
WORKDIR=`dirname "$(readlink -f "$0")"`
echo $WORKDIR
cd ${WORKDIR}/..

gen_dir=""
if [ -z "$ROOSET_GEN_DIR" ]; then gen_dir="gen"; else gen_dir=$ROOSET_GEN_DIR; fi

function clean {
  rm -rf ${gen_dir}
  mkdir -p ${gen_dir}/events
  mkdir -p ${gen_dir}/commands
  mkdir -p ${gen_dir}/tests
  mkdir -p ${gen_dir}/enums
  mkdir -p ${gen_dir}/exceptions
  mkdir -p ${gen_dir}/sources
}

export ROOSET_GEN_DIR=$gen_dir

# clean
node ./tools/src/index.js
