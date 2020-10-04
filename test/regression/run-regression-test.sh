#!/bin/bash
set -e

BASEDIR=$(dirname $0)
BINARY=${BASEDIR}/../../obj-convert

function test_obj_ascii_conversion {
    OBJ_FILE_LOCATION=$1
    EXPECTED_FILES_LOCATION=$2
    OUT_FILE_TYPE=$3
    EXTRA_PARAMS=$4
    TEST_FILES=$(find ${OBJ_FILE_LOCATION} -name '*obj' | sort)
    for FILE in ${TEST_FILES}
    do
        echo "Testing conversion of obj-file ${FILE} to stl-ascii"
        STL_FILE_CMP=$(echo ${FILE} | sed 's/\.obj/\.stl/g' | sed "s/obj-files/${EXPECTED_FILES_LOCATION}/g")
        STL_FILE_OUT=$(echo ${FILE} | sed 's/\.obj/\.stl/g' | sed "s/obj-files.*\//out\//g")

        echo "Executing: ${BINARY} obj ${FILE} ${OUT_FILE_TYPE} ${STL_FILE_OUT} ${EXTRA_PARAMS}"
        ${BINARY} obj ${FILE} ${OUT_FILE_TYPE} ${STL_FILE_OUT} ${EXTRA_PARAMS}
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
echo -e "\nRunning basic stl-ascii conversion..\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii stl-ascii
echo -e "\nRunning basic stl-bin conversion..\n\n"
test_obj_ascii_conversion data/obj-files-big expected-stl-to-bin stl-bin
