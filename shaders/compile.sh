#!/bin/sh

SHADER_DIR="${1:-$(dirname "$0")}"
if [ ! -d "$SHADER_DIR" ]; then
	echo "Shader directory '$SHADER_DIR' does not exist."
	exit 1
fi

SHADER_FILES=$(find "$SHADER_DIR" -type f \( -name "*.vert" -o -name "*.frag" \))
if [ -z "$SHADER_FILES" ]; then
	echo "No .vert or .frag files found in $SHADER_DIR or its subdirectories"
	exit 1
fi

for file in $SHADER_FILES; do
	extension="${file##*.}"
	output_file="${file%.*}_${extension}.spv"

	error_output=$(glslc "$file" -o "$output_file" 2>&1 >/dev/null)
	if [ -n "$error_output" ]; then
		echo "Error compiling $file:"
		echo "$error_output" | sed 's/^/  /'
		exit 1
	else
		echo "Compiled $file to $output_file"
	fi
done

echo "All shaders compiled successfully"

BUILD_DIR="$(dirname .)/build"
# TODO : Add release as well
