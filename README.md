# Project-LI3

Regarding IT Labs 3, we were challenged to make a C program that was responsible for managing large and small datasets of user's info.

This program would have to anwser some queries, for example, we needed to show in which flights a certain user is on.
We also had to mantain encapsulation and modulation, as well as creating a test program regarding the queries that were tested.

Our program can run in two different modes: the interactive mode, which we built using the ncurses library;
and the batch mode, where we only needed to anwser the queries that the user had in their file.

## Dependencies

You need to install ncurses that responsible for the interface

```bash
$ sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Run

1. Compile the program
```bash
$ make
```

2. Open the program in batch mode
```bash
$ ./programa-principal <dataset> <input/queries>
```

3. Open the program in interactive mode
```bash
$ ./programa-principal
```

4. Open the test program
```bash
$ ./programa-testes <dataset> <input/queries> <expected_output>
```

<h4>Grade: 18/20  </h4>
