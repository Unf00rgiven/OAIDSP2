#!/bin/bash

sudo ./waf prerequisites
./waf configure
TESSDATA_PREFIX="data/tessdata/" ./waf build run
