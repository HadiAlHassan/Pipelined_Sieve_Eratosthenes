#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <mpi.h>

void find_primes(int n) {
    int limit = (n > 6) ? (int)(n * (log(n) + log(log(n)))) : 15;
    bool *is_prime = malloc((limit + 1) * sizeof(bool));
    for (int i = 0; i <= limit; i++) is_prime[i] = true;

    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= sqrt(limit); i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    int count = 0;
    for (int i = 2; i <= limit && count < n; i++) {
        if (is_prime[i]) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");
    free(is_prime);
}

int main(int argc, char **argv) {
    int rank, size, n;
    double seq_start, seq_end, seq_time;
    double parallel_start, parallel_end, parallel_duration;
    int local_prime = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("We want %d primes\n", size);
        n = size;

        // Sequential version for timing
        seq_start = MPI_Wtime();
        find_primes(n);
        seq_end = MPI_Wtime();
        seq_time = seq_end - seq_start;

        printf("----------Sequential Code----------\n");
        printf("Sequential execution time: %f seconds\n", seq_time);
    }

    // Start parallel timing
    parallel_start = MPI_Wtime();
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        local_prime = 2;
        int x = 2;
        for (int i = 3; i <= n * n; i++) {
            if (i % 2 != 0) {
                MPI_Send(&i, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            }
        }
        int terminator = -1;
        MPI_Send(&terminator, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        printf("Process %d found prime: %d\n", rank, local_prime);

    } else {
        int x;
        MPI_Recv(&x, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_prime = x;

        int number;
        while (1) {
            MPI_Recv(&number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (number == -1) {
                if (rank < size - 1) {
                    MPI_Send(&number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
                break;
            }

            if (number % local_prime != 0) {
                if (rank < size - 1) {
                    MPI_Send(&number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
            }
        }
        printf("Process %d found prime: %d\n", rank, local_prime);
    }

    // End parallel timing
    parallel_end = MPI_Wtime();
    parallel_duration = parallel_end - parallel_start;

    if (rank == 0) {
        printf("----------Parallel Code----------\n");
        printf("Parallel execution time: %f seconds\n", parallel_duration);

        // Calculate speedup and efficiency
        float speedup = seq_time / parallel_duration;
        printf("Speedup: %f\n", speedup);
        printf("Efficiency: %f%%\n", (speedup / size) * 100);
    }

    MPI_Finalize();
    return 0;
}






//I want to find n primes
//Rank determines the position of the prime in the array
//I will use the sieve of Eratosthenes algorithm to find the primes

//I will use the following pseudocode to implement the algorithm
/*
recv(&x, Pi-1);
for (i = 0; i < n; i++) {
recv(&number, Pi-1);
if (number == terminator) break;
if (number % x) != 0) send(&number, Pi+1);
}
*/
