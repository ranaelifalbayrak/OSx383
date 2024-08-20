#!/bin/bash

sudo /sbin/losetup /dev/loop30 floppy.img
sudo mount /dev/loop30 /mnt
sudo cp src/kernel /mnt/kernel
sudo umount /dev/loop30
sudo /sbin/losetup -d /dev/loop30
