BF IDE
======

Operation
---------

### BF language & interpreter
 
* `+` Increment the current cell modulo 256.
* `-`  Decrement the current cell modulo 256.
* `<`  Shift left on the tape.
* `>`  Shift right on tape.
* `,`  Read one character of input, and write its ASCII value to the current cell.
* `.`  Write ASCII representation of current cell`s value to output.
* `[`  If current cell value is 0, continue executing after next `]`. Otherwise, continue executing.
* `]`  Terminate loop begun by `[`.

At cell index 0, `<` is an invalid command. `[` and `]` must be matched.

The IDE additionally supports breakpoints, indicated by `$`, which stop execution when running in either direction.

### bfide

Runs, steps, and examines the state of a BF program.

![bfide UI](ui.png)

1. BF program
2. I/O console: input is prefixed with `>`. Errors, if any, will also appear here.
3. Tape: right-infinite, each cell is mod-256.
4. Execution pointer: the next command to be executed. Highlights the following newline if at EOF.
5. Tape pointer: Shows the current cell of the tape.
6. The ASCII representation of the value of the cell.
7. The hexadecimal representation of the value of the cell.
8. Run/continue.
    * Forward: If at EOF, reset the program and begin running until EOF or a breakpoint. If already executing, but stopped, continue executing until EOF or a breakpoint.
    * Reverse: Run backwards from current execution pointer until beginning of program or a breakpoint.
9. Step. 
    * Forward: If at EOF, reset the program. Otherwise, execute the next instruction.
    * Backward: If at beginning of file, do nothing. Otherwise, step backwards one instruction.
10. Input field: Program input is taken from here. Input is consumed as it is used; only used input gets written to the I/O console.
11. Prompt: Determines execution behavior when the input field is blank, but the program needs more input. If User, focus goes to the input box, and execution stops until the user adds input. If Null, all inputs are nonblocking, and input commands when the input field is empty write the null character.
12. Save: saves to a file and sets it as the current file. The save button is deactivated when the disk file is up to date.
13. Open: loads a file and sets it as the current file.

The window title shows the name of the current file as well as an asterisk if it has been modified since the last save.

#### Command line invocation

`bfide [fltk-switch arg [fltk-switch arg [...]]] [filename]`

where `fltk-switch` is a [standard FLTK switch](https://www.fltk.org/doc-1.3/classFl.html#a1576b8c9ca3e900daaa5c36ca0e7ae48) and `filename` is the BF script to open, or `-` for stdin.

### clint <filename>

A simple interpreter. `clint` requires one argument, the filename of the BF script. It can be used as a shebang. The input (`,`) command blocks until input is available on stdin, output is to stdout, and error messages are on stderr. Unrecognized commands are ignored. Ca nbe built without FLTK.


Installation
------------

### Linux

1. Install [fltk 1.3](https://www.fltk.org/software.php) and make.

2. Compile. 

        cd /path/to/brainfide/
        make # CL interpreter and IDE
        make clint # CL interpreter only
        make bfide # IDE only

4. Run.

        make run # compile & run IDE
        ./bfide # IDE
        ./bfide filename.bf
        ./clint filename.bf # CL interpreter

### MinGW-w64

1. Install [fltk 1.3](https://www.fltk.org/software.php) and make.

        pacman -S mingw-w64-x86_64-fltk mingw-w64-x86_64-make

2. Compile.

        cd /path/to/brainfide/
        mingw32-make # CL interpreter and IDE
        mingw32-make clint # CL interpreter only
        mingw32-make bfide # IDE only

4. Run.

        mingw32-make run # compile & run IDE
        ./bfide # IDE
        ./bfide filename.bf
        ./clint filename.bf # CL interpreter


### Other platforms

Untested.

*This project is based in part on the work of the FLTK project (http://www.fltk.org).*