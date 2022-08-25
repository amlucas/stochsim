#!/bin/bash
# Copyright 2020 ETH Zurich. All Rights Reserved.

export PYTHONPATH=../../../build/src/uswim/bindings/:$PYTHONPATH

python3 -m unittest "$@"
