#!/bin/bash

path=${1:-./}Core/Inc/git_version.h

echo "getting git hash..."
hash=$(git rev-parse HEAD)
echo "writing git hash into" $path
echo -e $"#ifndef GIT_COMMIT_HASH\n#define GIT_COMMIT_HASH \"$hash\"\n#endif" > $path
