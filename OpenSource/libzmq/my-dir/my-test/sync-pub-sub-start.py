#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import os
import sys

def main():
    for _ in range(1,11,1):
        os.system("./syncsub &")
    os.system("./syncpub &")

if __name__ == '__main__':
    main()