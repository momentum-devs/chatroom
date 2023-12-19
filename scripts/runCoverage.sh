#!/bin/bash

ninja chatroom-ccov-all

llvm-cov-16 show `cat ccov/binaries.list` -instr-profile=ccov/all-merged.profdata > ../coverage.txt