[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/hM88s0XO)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=16792062&assignment_repo_type=AssignmentRepo)
# Assignment 5 - pbkdf1

## Pre-optimization Report
### Code Structure
The ISHA API, implemented in isha.h and isha.c, provides three main functions central to its hashing process. ISHAReset initializes the hashing context, ISHAInput processes byte input for hashing, and ISHAResult finalizes the hash by padding and computing a 160-bit output. These functions mimic the SHA-1 API but are simplified, particularly in the ISHAProcessMessageBlock function. The pbkdf1 function in pbkdf1.h and pbkdf1.c leverages ISHA to create a derived key (DK) of arbitrary length using a password and salt, compliant with PBKDF1 standards. pbkdf1_test files provide testing functions (test_isha and test_pbkdf1) to validate ISHA and PBKDF1 functionality. Finally, main.c features a time-reporting function critical for performance assessment as well as profiling.
### Function Tree

main.c <br>
├── main <br>
│   ├── run_tests <br>
│   │   ├── test_isha <br>
│   │   └── test_pbkdf1 <br>
│   ├── time_pbkdf1 <br>
│   │   ├── pbkdf1 <br>
│   │   │   ├── ISHAReset <br>
│   │   │   ├── ISHAInput <br>
│   │   │   │   └── ISHAProcessMessageBlock <br>
│   │   │   ├── ISHAResult <br>
│   │   │   │   ├── ISHAPadMessage <br>
│   │   │   │   │   └── ISHAProcessMessageBlock <br>
└   └   └   └   └   

### Code Characteristics
<table>
<tr>
<th>Compile Option</th><th>Speed (msec)</th><th>Size of .text (bytes)</th>
</tr>
<tr>
<td>-O0</td><td>694</td><td>12584</td>
</tr>
<tr>
<td>-O3</td><td>251</td><td>11496</td>
</tr>
<tr>
<td>-Os</td><td>310</td><td>9712</td>
</tr>
</table>
Although these numbers are identical to those listed in the document, all of them were found by running on Matt's hardware and using the "size" command on the .elf file.

### Function profiling:
<table>
<tr>
<th>Function</th><th>Static Profiling</th><th>PC Profiling</th><th>Runtime (msec)</th>
</tr>
<tr>
<td>ISHAProcessMessageBlock</td><td>4096</td><td>1585</td><td>317</td>
</tr>
<tr>
<td>ISHAPadMessage</td><td>4096</td><td>517</td><td>103.4</td>
</tr>
<tr>
<td>ISHAReset</td><td>4096</td><td>590</td><td>118</td>
</tr>
<tr>
<td>ISHAResult</td><td>4096</td><td>74</td><td>14.8</td>
</tr>
<tr>
<td>ISHAInput</td><td>4097</td><td>776</td><td>155.2</td>
</tr>
</table>
For the runtime calculation, we can approximate it by dividng the number of PC profiling counts by 5 to get the number of milliseconds that a given function has been running for. This method is not entirely accurate, as the addition of the profiling does impact the timing, so it's better to use the runtime as a relative measurement as opposed to an absolute one.


## Post-optimization Report
<table>
<tr>
<th>Compile Option</th><th>Speed (msec)</th><th>Size of .text (bytes)</th>
</tr>
<tr>
<td>-O0</td><td>486</td><td>14676</td>
</tr>
<tr>
<td>-O3</td><td>223</td><td>12660</td>
</tr>
<tr>
<td>-Os</td><td>284</td><td>11136</td>
</tr>
</table>

I focused primarily on the ISHAProcessMessageBlock function since it's the one that took the longest according to the PC profiling. By optimizing it the most, I could get the largest gains in overall performance.

List of optimizations:
* Removal of unnecessary bit-masking
    * Through out the code, there were a lot of unncessary bit masks that would be handled by rollover and type conversions
    * Removing those helped save on both execution time and code size
* Loop unrolling
    * In ISHAResult, there was a loop that only ran 5 times
    * By unrolling the loop, execution time could be shortened at the cost of code size
* Endian Swapping
    * The largest speed improvement of all, was changing from manual to __builtin_bswap32 for endian swapping
    * __builtin_bswap32 allows for a much higher degree of compiler optimization, since the compiler can recognize the exact purpose of the code section.