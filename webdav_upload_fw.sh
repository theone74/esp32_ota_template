#!/bin/bash

FILE=$1
FWBIN=build/_main.bin
INFEXT=txt
BINEXT=bin

[ -f .env ] && source .env

ver=`git -C src describe --tags --always --dirty 2>/dev/null || echo Unknown`
md5=`md5sum ${FWBIN} | awk '{ print $1 }'`

echo "$ver $md5" > /tmp/$FILE.$INFEXT

curl -X MKCOL -f "http://$WDLOGIN:$WDPASS@$WDHOST/$WDPATH" > /dev/null 2>&1
curl -T /tmp/$FILE.$INFEXT "http://$WDLOGIN:$WDPASS@$WDHOST/$WDPATH"
cp ${FWBIN} /tmp/$FILE.$BINEXT
curl -T /tmp/$FILE.$BINEXT "http://$WDLOGIN:$WDPASS@$WDHOST/$WDPATH"

echo done $FILE `cat /tmp/$FILE.$INFEXT`

rm -f /tmp/$FILE.$INFEXT /tmp/$FILE.$BINEXT