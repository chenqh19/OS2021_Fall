1. do q0, understand structure of code
2. understand what are the targets of four problems, find an algorithm for each
3. implement gradually, how to modify based on last question?





task1: 

in `main`, sequential instruction running

`init`, `update` add lock, all threads are safe

main function: run concurrently



task2:

in `init`, sequential embedding initialization

`init` has several embeddings, make them concurrent to accelerate `init`.



（use a queue to maintain the sequence before and after the slow function）



task3:

in `update`, update one user per instruction

update embeddings in epochs, data dependency: epoched by `iter_idx`



task4:

in `recommand`, using for loop, all recommended sequentially together in each call

recommend without be blocked by updates

must make sure the stuff recommended is after `iter_idx` executed