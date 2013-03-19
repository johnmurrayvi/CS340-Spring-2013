#!/bin/sh

PROG="../top-down"
# TESTFILES=( "song.txt" "test-dev-rand.txt" )
# TESTFLAGS=("" "i" "o" "l" "r")
# TESTREPLS=("1" "0x41" "b")
XT="output.txt"
LXT="log.txt"

check_exec() {
    TSTF=$1 # test file
    OUTF=$2 # output file
    IFLG=$3 # input flag
    OFLG=$4 # output flag
    LFLG=$5 # log flag
    RFLG=$6 # replacement flag

    # flags used
    PRNTFLG=""

    if [ $IFLG = 1 ] ; then
        PRNTFLG+="-i $TSTF"
    fi
    if ! [ -z $OUTF ] ; then
        PRNTFLG+="-o $OUTF"
    fi
    if ! [ -z $LFLG ] ; then
        PRNTFLG+="-l $LFLG"
    fi
    if ! [ -z $RFLG ] ; then
        PRNTFLG+="-r $RFLG"
    fi


    # diff original file to output
    # if replacement is specifed, they will differ,
    # so need to check for that at some point
    TSTIO=`diff $TSTF $OUTF | wc | awk '{ print $1 }'`

    if [ $TSTIO != 0 ] ; then
        echo "Test failed!"
    else
        echo "Test passed!"
    fi

    # only print the flags if there were any
    if [ -z $PRNTFLG ] ; then
        echo "File was $TSTF"
    else
        echo "File was $TSTF with flags \"$PRNTFLG\""
    fi
    echo ""
}

run_combos() {
    TESTFILE=$1
    FILEBN=${TESTFILE%.*}

    IOUT=$FILEBN-i-$XT
    $PROG -i $FILEBN > $IOUT
    check_exec $TESTFILE $IOUT 1 "" "" ""
    rm $IOUT

    ILOUT=$FILEBN-il-$XT
    ILLOG=$FILEBN-il-$LXT
    $PROG -i $FILEBN -l $ILLOG > $ILOUT
    check_exec $TESTFILE $ILOUT 1 "" "$ILLOG" ""
    rm $ILOUT $ILLOG

    IOOUT=$FILEBN-io-$XT
    $PROG -i $FILEBN -o $IOOUT
    check_exec $TESTFILE $IOOUT 1 "$IOOUT" "" ""
    rm $IOOUT

    OOUT=$FILEBN-o-$XT
    $PROG -o $OOUT
    check_exec $TESTFILE $OOUT 0 "$OOUT" "" ""
    rm $OOUT

    OLOUT=$FILEBN-ol-$XT
    OLLOG=$FILEBN-ol-$LOG
    $PROG -o $OLOUT -l $OLLOG
    check_exec $TESTFILE $OLOUT 0 "$OLOUT" "$OLLOG" ""
    rm $OLOUT $OLLOG

    LOUT=$FILEBN-l-$XT
    LLOG=$FILEBN-l-$LXT
    $PROG -l $LLOG > $LOUT
    check_exec $TESTFILE $LOUT 0 "" "$LLOG" ""
    rm $LOUT $LLOG
}

for f in song.txt ; do
    run_combos $f
done

exit 0
