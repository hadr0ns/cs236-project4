#!/bin/bash
NUM="2";
FOLDER="1-100";
TESTFILE="PassOff/"$FOLDER"/input"$NUM".txt";
ORIGPRINT="PassOff/"$FOLDER"/answer"$NUM".txt";
MYPRINT="passOffOut"$FOLDER"-"${NUM}".txt";

./main $TESTFILE > $MYPRINT;
diff $ORIGPRINT $MYPRINT;
