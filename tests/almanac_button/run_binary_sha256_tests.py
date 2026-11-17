#!/usr/bin/env python3
"""Cross-check identity SHA-256 against hashlib; no native plugin or game run."""
import argparse
import hashlib
import os
from pathlib import Path
import random
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--sanitize', action='store_true')
    args = parser.parse_args()
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('Compiler not found')
    build = ROOT / 'build/almanac_identity'
    build.mkdir(parents=True, exist_ok=True)
    exe = build / ('hash_check.exe' if os.name == 'nt' else 'hash_check')
    flags = ['-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic']
    if args.sanitize:
        flags += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    subprocess.run([compiler, *flags, str(ROOT / 'tests/almanac_button/almanac_binary_sha256_test.cpp'),
                    '-o', str(exe)], check=True, timeout=180)
    rng = random.Random(99117)
    fixtures = [b'', b'abc', b'a' * 1000000, bytes(range(256))]
    fixtures += [bytes(rng.randrange(256) for _ in range(size)) for size in
                 (1, 7, 55, 56, 57, 63, 64, 65, 119, 120, 121, 127, 128, 129, 4096, 65537)]
    count = 0
    for data in fixtures:
        expected = hashlib.sha256(data).hexdigest()
        for chunk in (1, 7, 64, 4096):
            result = subprocess.run([str(exe), str(chunk)], input=data,
                                    capture_output=True, check=True, timeout=30)
            actual = result.stdout.decode('ascii').strip()
            if actual != expected:
                raise AssertionError(f'SHA mismatch size={len(data)} chunk={chunk}: {actual} != {expected}')
            count += 1
    print(f'binary identity hash: {count} hashlib comparisons passed')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
