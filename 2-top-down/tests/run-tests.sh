#!/bin/bash

PROG=../top-down
# TESTFILES=( "song.txt" "test-dev-rand.txt" )
# TESTFLAGS=("" "i" "o" "l" "r")
# TESTREPLS=("1" "0x41" "b")
XT="output.txt"
LXT="log.txt"
TRF="tr-test.txt"

check_exec() {
    local TSTF=$1 # test file
    local OUTF=$2 # output file
    local IFLG=$3 # input flag
    local OFLG=$4 # output flag
    local LFLG=$5 # log flag
    local RFLG=$6 # replacement flag


    # diff filtered original file to output.
    # if replacement is specifed, they will differ,
    # so need to check for that at some point
    local TSTIO=`diff $TRF $OUTF | wc | awk '{ print $1 }'`

    if [ $TSTIO != 0 ] ; then
        echo "Test failed!"
    else
        echo "Test passed!"
    fi
    echo "File was $TSTF with flags:"
    echo "    -i $IFLG"
    echo "    -o $OFLG" 
    echo "    -l $LFLG"
    echo "    -r $RFLG"
    echo ""
}

run_combos() {
    TESTFILE=$1
    FILEBN=${TESTFILE%.*}

    tr -cd '\000-\177' < $TESTFILE > $TRF

    # no flags
    NFSOUT="$FILEBN-none-$XT"
    $PROG < $TESTFILE > $NFSOUT
    check_exec $TESTFILE "$NFSOUT" "" "" "" "" 
    rm $NFSOUT

    # input flag only
    IOUT="$FILEBN-i-$XT"
    $PROG -i $TESTFILE > $IOUT
    check_exec $TESTFILE "$IOUT" "$TESTFILE" "" "" ""
    rm $IOUT

    # input flag and log flag
    ILOUT="$FILEBN-il-$XT"
    ILLOG="$FILEBN-il-$LXT"
    $PROG -i $TESTFILE -l $ILLOG > $ILOUT
    check_exec $TESTFILE "$ILOUT" "$TESTFILE" "" "$ILLOG" ""
    rm $ILOUT $ILLOG

    # input flag and output flag
    IOOUT="$FILEBN-io-$XT"
    $PROG -i $TESTFILE -o $IOOUT
    check_exec $TESTFILE "$IOOUT" "$TESTFILE" "$IOOUT" "" ""
    rm $IOOUT

    # output flag only
    OOUT="$FILEBN-o-$XT"
    $PROG -o $OOUT < $TESTFILE
    check_exec $TESTFILE "$OOUT" "" "$OOUT" "" ""
    rm $OOUT

    # output flag and log flag
    OLOUT="$FILEBN-ol-$XT"
    OLLOG="$FILEBN-ol-$LXT"
    $PROG -o $OLOUT -l $OLLOG < $TESTFILE
    check_exec $TESTFILE "$OLOUT" "" "$OLOUT" "$OLLOG" ""
    rm $OLOUT $OLLOG

    # log flag only
    LOUT="$FILEBN-l-$XT"
    LLOG="$FILEBN-l-$LXT"
    $PROG -l $LLOG > $LOUT < $TESTFILE
    check_exec $TESTFILE "$LOUT" "" "" "$LLOG" ""
    rm $LOUT $LLOG

    rm $TRF
}

for f in song.txt ; do
    run_combos $f
done

exit 0
