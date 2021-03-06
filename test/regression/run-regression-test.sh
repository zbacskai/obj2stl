#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

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
        STL_FILE_CMP=$(echo ${FILE} | sed 's/\.obj/\.stl/g' | sed "s/obj-files/${EXPECTED_FILES_LOCATION}/g")
        STL_FILE_OUT=$(echo ${FILE} | sed 's/\.obj/\.stl/g' | sed "s/obj-files.*\//out\//g")

        echo "Executing: ${BINARY} obj ${FILE} ${OUT_FILE_TYPE} ${STL_FILE_OUT} ${EXTRA_PARAMS}"
        ${BINARY} obj ${FILE} ${OUT_FILE_TYPE} ${STL_FILE_OUT} ${EXTRA_PARAMS}
        RESULT=$(diff ${STL_FILE_CMP} ${STL_FILE_OUT} 2>&1)
        if [ -z "${RESULT}" ]
        then
          echo -e "${GREEN}Passed${NC}"
        else
          echo -e "${RED}Failed${NC}"
        fi
        done
}

function report_result {
    MSG=$1
    RES=$2
    EXPECTED=$3

    TEST_STR=$(echo ${RES} | grep "${EXPECTED}")

    if [ -z "${TEST_STR}" ]
    then
        echo -e "${MSG} ${RED}Failed${NC}"
    else
        echo -e "${MSG} ${GREEN}Passed${NC}"
    fi
}

function test_point_inside {
    RES1=$(${BINARY}  obj data/obj-files/test_08_box.obj stl-ascii data/out/test.stl --point_in_model 0.0,0.0,0.0)
    RES2=$(${BINARY}  obj data/obj-files/test_05_skull.obj stl-ascii data/out/test.stl --point_in_model 0.0,-3.5,1.0)
    RES3=$(${BINARY}  obj data/obj-files/test_01_hand.obj stl-ascii data/out/test.stl --point_in_model 0.0,0.0,0.0)

    report_result "Test: box, point 0.0,0.0,0.0" "${RES1}" inside
    report_result "Test: skull, point 0.0,-3.5,1.0" "${RES2}" inside
    report_result "Test: hand, point 0.0,0.0,0.0" "${RES3}" outside
}

echo "Running regression tests!"
echo -e "\nRunning basic stl-ascii conversion.\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii stl-ascii
echo -e "\nRunning basic stl-bin conversion.\n\n"
test_obj_ascii_conversion data/obj-files-big expected-stl-to-bin stl-bin
echo -e "\nRunning basic stl-ascii conversion. - translate\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii-translate stl-ascii '--transformations translation=1.2,3.1,0.0'
echo -e "\nRunning basic stl-ascii conversion. - rotate\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii-rotate stl-ascii '--transformations rotate=-23.1,67.1,11.0'
echo -e "\nRunning basic stl-ascii conversion. - scale\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii-scale stl-ascii '--transformations scale=2,1,0'
echo -e "\nRunning basic stl-ascii conversion. - translation/scale/rotate\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii-all-trans-1 stl-ascii '--transformations translation=1.2,3.1,0.0/scale=2,1,0/rotate=-23.1,67.1,11.'
echo -e "\nRunning basic stl-ascii conversion. - scale/rotate/translation\n\n"
test_obj_ascii_conversion data/obj-files expected-stl-to-ascii-all-trans-2 stl-ascii '--transformations scale=2,1,0/rotate=-23.1,67.1,11.0/translation=1.2,3.1,0.0'
echo -e "\nRunning - test if point inside\n\n"
test_point_inside
