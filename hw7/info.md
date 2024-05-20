# Student Info

**Name**: 陳羿閔 (B081020008)  
**Date**: 2023-11-25  
**Description**: Testing the performance of different sorting algorithms

## System and Compiler Info

### General System Info

_Info retrieved on `2023-11-25 15:50` through `neofetch`_

- **OS**: macOS 14.1 23B74 arm64
- **Host**: MacBookPro18, 3
- **Kernel**: 23.1.0
- **Memory**: 12687MiB / 16384MiB
- **Uptime**: 24 days, 57 min

### Frequency of CPU 0 - 7

_Info retrieved on `2023-11-25 15:50` through `sudo /usr/bin/powermetrics -s cpu_power -n 1 | grep CPU`_

- **CPU 0**: 1315 MHz
- **CPU 1**: 1307 MHz
- **CPU 2**: 2185 MHz
- **CPU 3**: 1695 MHz
- **CPU 4**: 1873 MHz
- **CPU 5**: 2118 MHz
- **CPU 6**: 1252 MHz
- **CPU 7**: 1284 MHz

### Compiler Info

_Info retrieved on `2023-11-25 15:50` through `gcc -v`_

- **Version**: Apple clang version 15.0.0 (clang-1500.0.40.1)
- **Target**: arm64-apple-darwin23.1.0
- **Thread model**: posix
- **InstalledDir**: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

## Benchmark Info

### About Test Data

- The following `python` script was used to generate the test data:

  ```python
  import random

  rounds = [
      100,
      500,
      1000,
      5000,
      10000,
      50000,
      100000,
      500000,
  ]


  with open("input.txt", "a") as f:
      for round in rounds:
          f.write(str(round) + "\n")
          for i in range(round):
              f.write(str(random.randint(1, round + 1)) + "\n")
  ```

### Results

_The program was compiled with `g++ -std=c++20 -O3 -o main main.cpp`_

#### Algorithms:

- selection sort
- heap sort
- quick sort
- quick sort (qsort c lib)
- sort (c++ lib)

#### Table:

| Data   | Selection Sort | Heap Sort  | Quick Sort | qsort (c)  | sort (c++) |
| ------ | -------------- | ---------- | ---------- | ---------- | ---------- |
| 100    | 0.000027 s     | 0.000015 s | 0.000014 s | 0.000016 s | 0.000005 s |
| 500    | 0.000748 s     | 0.000110 s | 0.000120 s | 0.000110 s | 0.000025 s |
| 1000   | 0.003034 s     | 0.000216 s | 0.000200 s | 0.000214 s | 0.000043 s |
| 5000   | 0.060168 s     | 0.000814 s | 0.000791 s | 0.000814 s | 0.000160 s |
| 10000  | 0.185903 s     | 0.001763 s | 0.001668 s | 0.001889 s | 0.000366 s |
| 50000  | 4.288537 s     | 0.010398 s | 0.009998 s | 0.010919 s | 0.002122 s |
| 100000 | 17.086272 s    | 0.022201 s | 0.021739 s | 0.022248 s | 0.004127 s |
| 500000 | TLE            | 0.127051 s | 0.115117 s | 0.128890 s | 0.025860 s |
