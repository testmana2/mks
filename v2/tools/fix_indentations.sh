#!/bin/sh

# this script convert tab indentation to space indentation with a tab width of 4 spaces.

if [ -z "$1" ]; then
    echo "Please give the folder to check as first parameter."
    exit 1
fi

CUR_PATH=$PWD

cd "$1"

find . -type f -name "*.h*" -o -name "*.c*" -o -name "*.ini" -o -name "*.txt" -o -name "*.rc" -o -name "*.conf" -o -name "*.iss" -o -name "*.py" -o -name "*.sh" -o -name "*.mks" -o -name "*.pri" -o -name "*.pro" -o -name "*.xup" | grep -v .svn | while read line
#find . -type f -name "*" | grep -v .svn | grep -v *.png | while read line
do
    expand --tabs=4 "$line" > "$line.new"
    mv "$line.new" "$line"
done

cd "$CUR_PATH"