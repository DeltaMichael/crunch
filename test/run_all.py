#!/usr/bin/python3

import subprocess
import os

def checkeq(expected, result, lineno):
    if expected != result:
        return f'Line {lineno}: Expected {expected}, but got {result}'
    return None

filenames = [name for name in os.listdir('./') if 'expected' in name]
for name in filenames:
    expected = []
    errors = []
    with open(name) as f:
        expected.append(f.readline().strip())
    binary = name.split('.')[0]
    out = subprocess.run(['../crunch', f'./{binary}'], stdout = subprocess.PIPE).stdout.decode('utf-8')
    for i, result in enumerate(out.split('\n')):
        err = checkeq(expected[i], result, i)
        if err:
            errors.append(err)
    if errors:
        print(f"TEST {binary}... FAILED")
        for err in errors:
            print(err)
    else:
        print(f"TEST {binary}... SUCCESS")
