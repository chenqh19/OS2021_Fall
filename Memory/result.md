#### Q1

| task1 | task2 | task3 |
| ----- | ----- | ----- |
| 2ms   | 168ms | 36ms  |



#### Q2

| task1 | task2 | task3 |
| ----- | ----- | ----- |
| 2ms   | 166ms | 39ms  |

Clock algorithm takes more time to execute in task3 but less time in task2. The time of two algorithms doesn't vary a lot. It may because that the task load isn't so big that the time difference is mainly because the code complexity and some random things.

#### Q3

| mma_sz = | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | 10   |
| -------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| CLOCK    | 164  | 174  | 169  | 165  | 169  | 166  | 166  | 169  | 168  | 168  |
| FIFO     | 163  | 166  | 164  | 169  | 168  | 166  | 165  | 166  | 164  | 166  |

The average execution time of CLOCK algorithm is slightly slower than FIFO algorithm. That may because the improvement of the replacement policy doesn't contribute a lot to the execution time, so the complexity differences of two algorithms leads to this result.

#### Q4

| thread_num = | 10   | 11   | 12   | 13   | 14   | 15   | 16   | 17   | 18   | 19   | 20   |
| ------------ | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **Time =**   | 32   | 103  | 162  | 166  | 164  | 151  | 194  | 166  | 211  | 217  | 230  |

When thread number increases, the execution time increases in a small amount. Since more threads indicates more task load, the execution time will increases. And due to the implementation of concurrency, the execution time only increases a small amount. Some vacillation is because the randomness in multi-thread execution.
