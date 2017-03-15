#!/bin/bash

find -type f -name '*.rpt' | while read file; do
	mkdir -p "bak/$@/$(dirname "$file")"
	echo "bak/$@/$file"
	cp -a "$file" "bak/$@/$file"
done
