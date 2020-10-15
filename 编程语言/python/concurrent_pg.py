#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import requests
import concurrent.futures
from concurrent.futures import ThreadPoolExecutor, as_completed

def task_run(url):
    try:
        rep = requests.get(url)
        if rep.status_code != 200:
            print("%s http status %d".format(url, rep.status_code))
    except requests.exceptions.RequestException as e:
        print("%s except" % url)
        return e

def replay_request(req_file, domain):
    lines = []
    with open(req_file) as reader:
        lines = [line.rstrip() for line in reader]

    threads = []
    with ThreadPoolExecutor(max_workers=20) as executor:
        for line in lines:
            url = domain + line
            threads.append(executor.submit(task_run, url))

        finished = concurrent.futures.wait(threads)
        print(finished)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage : %s {filename}' % sys.argv[0])
        sys.exit(1)

    domain = "xxx"
    replay_request(sys.argv[1], domain)