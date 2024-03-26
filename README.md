# Tape sorting
This program sorts tapes consisting of integers from input tape to output tape.
Tapes are emulated using text files.


## Building
1. Clone the repo.
2. Create build dir and move there: mkdir build && cd build.
3. Configure cmake: cmake .. If you wand to build tests specifiy variable BUILD_TESTS : -DBUILD_TESTS=ON.
4. Build app : cmake --build .


## Usage
### i/O file formats
The tape class is emulated by interacting with file streams. 
An integer can occupy a maximum of 11 characters, so 11 bytes will be allocated for each number in the file. Empty elements are considered zeros.
Therefore, each individual number is read into a buffer of 11 characters. As a last resort, a valid 3-element tape format looks like this:
<code>-2147483648-2147483648-2147483648</code>
Which matches  `{-2147483648, -2147483648, -2147483648}`


### Format of a timings configuration file
Configuration file with tape config consists of lines separated by the '\n' character. Individual fields within a line are separated by '='.
Settings can be specified in any order; invalid keys and values ​​will be ignored.

Field names:
* 'r' or "read" for read timing;
* 'w' or "write" for write timing;
* "mv" or "move" for move_left and move_right timing;
* "rd" or "rewind" for rewind timing;

Example: 
```
r     = 1
write \t\t = 214  \t\t
mv = -1000
rd=10
invalid_key = 10
```
Will match  `{read=1, write=214, move=0, rewind=10}`

### User interface:
```
Usage:
  ./tape [OPTION...] <input> <output>

  -h, --help        Show help
  -i, --input arg   Input file
  -o, --output arg  Output file (default: output.txt)
  -c, --config arg  Config file
  -b, --bytes arg   Memory (default: 40)
  -t, --tapes arg   Temporary tapes
```