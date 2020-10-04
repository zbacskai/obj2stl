#/bin/bash

BASEDIR=$(dirname $0)

function run_regression {
    echo "Running regression tests"
    ${BASEDIR}/regression/run-regression-test.sh
}

function run_unit {
    echo "Unit-test has not yet been implemented due to short timeframe."
    echo "TODO: implement a set of Google-test based tests"
}

run_regression
run_unit
