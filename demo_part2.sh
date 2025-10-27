#!/usr/bin/env bash

FILE_TEST=./TestCasesPart2/*

OUTPUT_DIR="OutputsPart2"

mkdir -p $OUTPUT_DIR

for file in $FILE_TEST
do
  file_name="${file##*/}"
  no_ext="${file_name%.tips}"
  ./parse "$file" 2>&1 | tee "$OUTPUT_DIR/${no_ext}.out" 2>&1
done

