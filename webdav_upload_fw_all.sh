#!/bin/bash

while read -r id; do
	./webdav_upload_fw.sh $id
done < devices.txt