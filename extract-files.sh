#!/bin/bash

VENDOR=samsung
DEVICE=a5y17lte

BASE=../../../vendor/$VENDOR/$DEVICE/proprietary
rm -rf $BASE/*

for FILE in `egrep -v '(^#|^$)' proprietary-files.txt`; do
    OLDIFS=$IFS IFS=":" PARSING_ARRAY=($FILE) IFS=$OLDIFS
    FILE=${PARSING_ARRAY[0]}
    DEST=${PARSING_ARRAY[1]}
    if [ -z $DEST ]
    then
        DEST=$FILE
    fi
    DIR=`dirname $FILE`
    if [ ! -d $BASE/$DIR ]; then
        mkdir -p $BASE/$DIR
    fi

    if [ -z "$STOCK_ROM_DIR" ]; then
        adb pull /system/$FILE $BASE/$DEST
        # if file does not exist try destination
        if [ "$?" != "0" ]
        then
            adb pull /system/$DEST $BASE/$DEST
        fi
    else
        cp $STOCK_ROM_DIR/$FILE $BASE/$DEST
    fi
done

./setup-makefiles.sh
