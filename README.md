# MPI
Some MPI Samples

### Blowfish results on VM cluster:

Brief description of how the benchmarks were performed:
- Initialization: time to read the input file, calculate the offsets for splitting the file into chunks of data (if needed), allocate memory and initialize blowfish parameters;
- Computation: time to encrypt, decrypt, write data to memory and send data to rank 0 (gather);
- Total: total application time including memory deallocation and verification of correctness. 

__SERIAL 5MB FILE__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f file.txt -k bioinformatics (family=openmpi3)
File size: 5572792 bytes split into 696599 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.038672s
  Computation:    0.063190s
  Total:          0.102423s
```
__SERIAL 500MB FILE__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f book.txt -k bioinformatics (family=openmpi3)
File size: 500563104 bytes split into 62570388 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 1.820641s
  Computation:    5.905069s
  Total:          7.737537s
```
__SERIAL 1GB FILE__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f tome.txt -k bioinformatics (family=openmpi3)
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 3.826321s
  Computation:    11.841680s
  Total:          15.692811s
```
In the serial version, the compute time is dominant for all problem sizes. The time of verification is negligible.

__MPI 5MB -n 4 -N 4__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f file.txt -k bioinformatics (family=openmpi3)
File size: 5572792 bytes split into 696599 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.026710s
  Computation:    0.029338s
  Total:          0.056504s
```
__MPI 500MB -n 4 -N 4__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f book.txt -k bioinformatics (family=openmpi3)
File size: 500563104 bytes split into 62570388 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 1.741193s
  Computation:    2.313624s
  Total:          4.065768s
```
__MPI 1GB -n 4 -N 4__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f tome.txt -k bioinformatics (family=openmpi3)
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 3.389017s
  Computation:    4.800958s
  Total:          8.211742s
```
With 4 MPI ranks (4 nodes) and a single thread per node, there's some speedup, although not equal to the number of nodes. The initialization times remain the same as in the serial version because this code has rank 0 read the whole file for verification. There is no message passing in the initialization phase (no scatter). Computation time is not as dominant as in the serial version.

__MPI+OpenMP 5MB -n 4 -N 4 -c 4__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f file.txt -k bioinformatics (family=openmpi3)
File size: 5572792 bytes split into 696599 blocks of 64 bits
Executing with 4 OpenMP threads and 4 MPI ranks
PASSED
  Execution times:
  Initialization: 0.020857s
  Computation:    0.043846s
  Total:          0.065160s
  ```
 __MPI+OpenMP 500MB -n 4 -N 4 -c 4__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f book.txt -k bioinformatics (family=openmpi3)
File size: 500563104 bytes split into 62570388 blocks of 64 bits
Executing with 4 OpenMP threads and 4 MPI ranks
PASSED
  Execution times:
  Initialization: 1.866346s
  Computation:    1.243591s
  Total:          3.121033s
  ```
__MPI+OpenMP 1GB -n 4 -N 4 -c 4__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f tome.txt -k bioinformatics (family=openmpi3)
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
Executing with 4 OpenMP threads and 4 MPI ranks
PASSED
  Execution times:
  Initialization: 4.590790s
  Computation:    2.585831s
  Total:          7.198708s
``` 
With 4 MPI ranks (4 nodes) and 4 threads per node, the achieved speedup in relation to the serial version is good - but not great - considering that this is a hybrid code. Results make clear that this is an IO-bound application. Initialization times are now dominant.

### Blowfish results on Nucleus:

__SERIAL 5MB FILE__
```sh
File size: 5572792 bytes split into 696599 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.056338s
  Computation:    0.088864s
  Total:          0.145710s
```
__SERIAL 500MB FILE__
```sh
File size: 500563104 bytes split into 62570388 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.548407s
  Computation:    7.980852s
  Total:          8.540638s
```
__SERIAL 1GB FILE__
```sh
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 1.124930s
  Computation:    15.953582s
  Total:          17.101078s
```
Note the decrease in initialization times because of the parallel file system in nucleus (the 5MB is an exception). Compute times are worse, tough.

__MPI 5MB -n 4 -N 4__
```sh
File size: 5572792 bytes split into 696599 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.009882s
  Computation:    0.025483s
  Total:          0.035448s
```
__MPI 500MB -n 4 -N 4__
```sh
File size: 500563104 bytes split into 62570388 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.449274s
  Computation:    2.200417s
  Total:          2.660147s
```
__MPI 1GB -n 4 -N 4__
```sh
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.944908s
  Computation:    4.377276s
  Total:          5.342119s
```
With 4 MPI ranks (4 nodes) and a single thread per node, good speedup is achieved, very similar to the number of nodes. Those are better values than those of the VM cluster, due to Infiniband. The initialization times remain the same as in the serial version because this code has rank 0 read the whole file for verification.

__MPI+OpenMP 5MB -n 4 -N 4 -c 16__
```sh
File size: 5572792 bytes split into 696599 blocks of 64 bits
Executing with 16 OpenMP threads and 4 MPI ranks
PASSED
  Execution times:
  Initialization: 0.007641s
  Computation:    0.008256s
  Total:          0.015984s
```
 __MPI+OpenMP 500MB -n 4 -N 4 -c 16__
```sh
File size: 500563104 bytes split into 62570388 blocks of 64 bits
Executing with 16 OpenMP threads and 4 MPI ranks
PASSED
  Execution times:
  Initialization: 0.489728s
  Computation:    0.393478s
  Total:          0.893121s
```
__MPI+OpenMP 1GB -n 4 -N 4 -c 16__
```sh
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
Executing with 16 OpenMP threads and 4 MPI ranks
PASSED
  Execution times:
  Initialization: 0.982534s
  Computation:    0.747577s
  Total:          1.749635s
``` 
With 4 MPI ranks (4 nodes) and 16 threads per node, there's some more speedup, although not ideal. Once again, results demonstrate that this is an IO-bound application. Initialization times are now dominant.
