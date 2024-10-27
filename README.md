# Pipelined Sieve of Eratosthenes using MPI

The following code represents a pipelined parallel version of the sieve of eratosthenes using the MPI library, 
to find n primes using n processors
This is a Type 2 Pipeline

The concept of the pipeine is simple:
- Process 0 (Master node) will start off with the first prime number (2)
-- Then it will send to the next process the first non multiple of 2, and keep sending any number that also isnt a multiple of 2
- Process 1 will receive the next prime number (3) and filter out non multiples of 3
- Following this logic, each process will filter out the composite numbers which are multiples of the prime number that process is storing
- The first number each process receives will be the next prime number (first non multiple of 2 is 3, first non multiple of 3 is 5, etc...)
- We will keep doing so until the N processes have a prime number stored.

# Pseudocode for Master Node (Rank 0)

1. **Initialize**:
   - Set `local_prime` to 2 (the first prime).
   - Define a variable `x` as 2.

2. **Send Odd Numbers**:
   - For each integer `i` from 3 up to `n * n`:
     - If `i` is not a multiple of 2, send `i` to the next process (`rank + 1`).

3. **Send Terminator**:
   - Define `terminator` as -1.
   - Send `terminator` to the next process to signal the end.

4. **Print Prime**:
   - Print the prime found by this process (`local_prime`).

# Pseudocode for a Process \( P_i \)

1. **Receive Initial Prime**:
   - Receive a prime number `x` from the previous process (`rank - 1`).
   - Set `local_prime` to `x`.

2. **Filter and Forward Numbers**:
   - **Loop**: Continue until a terminator is received:
     - Receive a number `number` from the previous process (`rank - 1`).
     - **If** `number` is the terminator:
       - Forward the terminator to the next process (`rank + 1`) if this is not the last process.
       - **Break** out of the loop.
     - **Else If** `number` is not divisible by `local_prime`:
       - Forward `number` to the next process (`rank + 1`) if this is not the last process.

3. **Print Local Prime**:
   - After receiving the terminator, print the prime number `local_prime` for this process.

   
