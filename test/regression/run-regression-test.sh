#!/bin/bash
set -e

BASEDIR=$(dirname $0)
BINARY=${BASEDIR}/../../obj-convert

function test_obj_ascii_conversion {
    TEST_FILES=$(find data/obj-files -name '*obj' | sort)
    for FILE in ${TEST_FILES}
    do
        echo "Testing conversion of obj-file ${FILE} to stl-ascii"
        STL_FILE_CMP=$(echo ${FILE} | sed 's/\.obj/\.stl/g' | sed 's/obj-files/expected-stl-ascii-no-conv/g')
        STL_FILE_OUT=$(echo ${FILE} | sed 's/\.obj/\.stl/g' | sed 's/obj-files/out/g')

        ${BINARY} obj ${FILE} stl-ascii ${STL_FILE_OUT}
        RESULT=$(diff ${STL_FILE_CMP} ${STL_FILE_OUT})
        if [ -z "${RESULT}" ]
        then
          echo "Passed"
        else
          echo "Failed"
        fi
        done
}

echo "Running regression tests!"
test_obj_ascii_conversion
