#!/bin/bash

path=${1:-./}Core/Inc/git_version.h

echo "getting git hash..."
hash=$(git rev-parse HEAD)
hashval=$(echo $hash | cut -c1-8)
echo "writing git hash into" $path
echo -e "#ifndef GIT_COMMIT_HASH
	#define GIT_COMMIT_HASH \"$hash\"
#endif
#ifndef GIT_COMMIT_HASH_VALUE
	#define GIT_COMMIT_HASH_VALUE 0x$hashval
#endif" > $path
