#!/bin/sh

gcc main.c -o get_key
sudo chown root:root ./get_key
sudo chmod +s ./get_key

