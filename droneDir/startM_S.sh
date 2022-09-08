#!/bin/bash

i2cset -y 2 0x68 0x6B 0x00

config-pin p2.28 pruout
config-pin p2.30 pruout
config-pin p2.32 pruout
config-pin p2.34 pruout
