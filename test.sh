#!/bin/bash
NUM="20";
TESTFILE="test-cases/in"${NUM}".txt";
ORIGPRINT="test-cases/out"${NUM}".txt";
MYPRINT="my-out"${NUM}".txt";

./main $TESTFILE > $MYPRINT;
diff $ORIGPRINT $MYPRINT;
