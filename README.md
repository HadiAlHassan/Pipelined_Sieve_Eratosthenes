# Pipelined Sieve of Eratosthenes using MPI

The following code represents a pipelined parallel version of the sieve of eratosthenes using the MPI library, 
to find n primes using n processors
This is a Type 2 Pipeline

Obviously most of this was generated by ChatGPT, automation exists for a reason.
# Sieve of Eratosthenes

The **Sieve of Eratosthenes** is an ancient and efficient algorithm for finding all prime numbers up to a specified integer \( n \). It works by iteratively marking the multiples of each prime starting from \( 2 \).

## Algorithm Steps

1. **Create a List of Numbers**:
   - Create a list of booleans (or an array) from \( 2 \) to \( n \).
   - Initialize all values as `true`, indicating that all numbers are assumed to be prime initially.

2. **Mark Non-Primes**:
   - Start with the first prime number, \( p = 2 \).
   - Mark all multiples of \( p \) (i.e., \( 2p, 3p, \dots \)) as `false` because multiples of \( p \) are not prime.
   - Move to the next unmarked number (next `true` value), which is the next prime.
   - Repeat the marking process for each new prime number up to \( \sqrt{n} \).

3. **Collect Primes**:
   - After marking all multiples, the remaining numbers marked as `true` are prime numbers.

## Example: Finding Primes up to 30

1. Start with a list from \( 2 \) to \( 30 \):
2. Begin with \( p = 2 \):
- Mark multiples of \( 2 \): \( 4, 6, 8, 10, ... \), up to \( 30 \).

3. Move to the next unmarked number, \( p = 3 \):
- Mark multiples of \( 3 \): \( 6, 9, 12, 15, ... \), up to \( 30 \).

4. Continue to \( p = 5 \):
- Mark multiples of \( 5 \): \( 10, 15, 20, ... \), up to \( 30 \).

5. Repeat for remaining primes up to \( \sqrt{30} \) (approximately 5.5).

6. Collect all numbers still marked as `true`:
- The prime numbers up to 30 are: \( 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 \).

## Complexity and Efficiency

- **Time Complexity**: \( O(n \log \log n) \), which is very efficient for finding primes up to large values of \( n \).
- **Space Complexity**: \( O(n) \), as it requires storage of boolean values for each integer up to \( n \).

## Advantages and Use Cases

- **Efficient**: The Sieve of Eratosthenes is highly efficient for generating a list of primes up to a large number.
- **Easy to Implement**: The algorithm's steps are straightforward and easy to implement.


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
