# MPI
Some MPI Samples

### Blowfish results on VM cluster:

__SERIAL VERSION 5MB FILE__
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
__SERIAL VERSION 50MB FILE__
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
__SERIAL VERSION 1GB FILE__
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
__MPI VERSION 5MB -n 8 -N 2__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f file.txt -k bioinformatics (family=openmpi3)
File size: 5572792 bytes split into 696599 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 0.020651s
  Computation:    0.027808s
  Total:          0.048894s
```
__MPI VERSION 50MB -n 8 -N 2__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f book.txt -k bioinformatics (family=openmpi3)
File size: 500563104 bytes split into 62570388 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 1.563627s
  Computation:    1.543340s
  Total:          3.118335s
```
__MPI VERSION 1GB -n 8 -N 2__
```sh
[prun] Master compute host = compnode0
[prun] Resource manager = slurm
[prun] Launch cmd = mpirun ./blowfish -f tome.txt -k bioinformatics (family=openmpi3)
File size: 1001126208 bytes split into 125140776 blocks of 64 bits
PASSED
  Execution times:
  Initialization: 4.457665s
  Computation:    2.775760s
  Total:          7.255214s
```
