#!/bin/sh

# Executable testing script
# Purpose of this script is to run an executable to be tested with files in a
# directory and compare the stdout of the executable to files containing the
# expected output
#
# Usage: ./run-tests.sh <executable> <testing-directory>
#
# This searches <testing-directory> for .sc files, runs the <executable> with
# all files ending with '.sc' as argument and compares the output to the files
# ending with '.expected'.
#
# Depends: grep, tee, diff



# Run actual testing
#
# $1: the executable filename
# $2: the basename of the test filename
#
# returns: 0 if the test passed, 1 if the test failed and 2 if unsure
run_test()
{
    # compare the output of $1 when called with $2.sc to $2.expected
    "$1" "$2.sc" 2>&1 | tee "$TEMP_OUT_FILE" | \
        diff --side-by-side - "$2.expected" 2>&1 > "$TEMP_DIFF_OUT"

    return $?
}

# first argument is executable file name
if [ ! -f "$1" ]; then
    echo "First argument must be the path to the executable to be tested!"
    exit 1
else
    EXFILE="$1"
    # if somebody provided the name of the executable without any directories,
    # prepend './' to be able to execute the file
    if [ "$EXFILE" = $(basename "$EXFILE") ]; then
        EXFILE="./$EXFILE"
    fi
fi

# second argument is the testing directory
if [ ! -d "$2" ]; then
    echo "Second argument must be the testing directory!"
    exit 1
else
    TESTDIR="$2"
fi

# we need a temporary file for the program output
# TEMP_OUT_FILE=$(mktemp)
TEMP_OUT_FILE="/dev/null"
TEMP_DIFF_OUT=$(mktemp)

# retrieve only those files that end with .sc
FILEPAT='.\+\.sc'

# retrieve all test files from testing directory
ifs="$IFS"
IFS="
"

TESTFILES=$(ls "$TESTDIR" | grep $FILEPAT )

IFS="$ifs"

for TFILE in $TESTFILES
do
    T_BASENAME="${TFILE%.sc}"
    printf "Running $T_BASENAME... "

    run_test "$EXFILE" "$TESTDIR/$(basename $TFILE .sc)"
    if [ $? -eq 0 ]
    then
        printf '\33[32m passed. \33[0m\n'
    else
        printf '\33[31m failed! \33[0m\n'
        # printf 'output:\n'
        # cat "$TEMP_OUT_FILE"
        printf 'diff:\n'
        cat "$TEMP_DIFF_OUT"
        printf '\n'
    fi
done

# rm -f "$TEMP_OUT_FILE"
rm -f "$TEMP_DIFF_OUT"

exit 0

