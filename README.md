###Motivation
I approached this problem from the perspective of wanting to demonstrate that I can use data structures appropriately to write efficient code. My implementation uses a series of hashmaps to provide constant time lookups, and prevent me from having to scan through the system of equations more than once. I also solved a slightly more general problem than the one specified in the instructions. In my solution variables don't have to have unsigned values (but other terms still must still be unsigned), and the left side of the equation is not just limited to a variable name, but can be an expression of the same form as the right side.

### High Level Overview
The implementation is based primarily around two hashmaps: equation_map and solution_map. 

- equation_map<string, list of equation> is a mapping from each variable to a vector of every equation in the system that has this variable in it
- solution_map<string, double> is a map of all the variables that have been solved for so far. It maps from a variable name to that variable's known value.

Here is some psudocode explaining how the algorithm works:

```
1  For each equation E:
2     for each variable v in E:
3         if solution_map contains v:
4             replace v with the value solution_map[v]
5         else:
6             add mapping from v -> E to equation_map
7     if E only contains one variable w and value a:
9         add mapping from w -> a to solution_map.
10        look up w in equation_map to get vector of equations F that contain w.
11        for each equation in F:
12            update F by replacing variable w with value a.
13            jump to (7) with E=F
14  print contents of solution_map in alphabetical order 
```

Each equation is represented as an instance of class EquationNode. equation_map and solution_map are both shared static members of class EquationNode.

 
### Build and Execution Instructions
to compile the program, just run `make` from the root directory. The resulting executable is in `bin/main`. The program takes a file name as a parameter. Here is an example command from the root directory:
```
./bin/main data/equations2.txt
```

To check for memory leaks, just prepend `valgrind --leak-check=full --show-leak-kinds=all` to the command, like so:
```
/Projects/tesla-c-$ valgrind --leak-check=full --show-leak-kinds=all ./bin/main data/equations2.txt
...
...
==13722== LEAK SUMMARY:
==13722==    definitely lost: 0 bytes in 0 blocks
==13722==    indirectly lost: 0 bytes in 0 blocks
==13722==      possibly lost: 0 bytes in 0 blocks
==13722==    still reachable: 72,704 bytes in 1 blocks
==13722==         suppressed: 0 bytes in 0 blocks
==13722== 
==13722== For counts of detected and suppressed errors, rerun with: -v
==13722== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```

### Example Output
Input:
```
5 = b
c = 2 + 5 + b
d + d + c = d
e + 5 + d = f + c
g = 3.3
h = g + g + 1
7 + m = g + h
f + f = 10
```
Output:
```
b = 5
c = 12
d = -12
e = 24
f = 5
g = 3.3
h = 7.6
m = 3.9
```

### Thoughts On The Assignment
I definitely spent more than 3 hours on this project. My c++ is a bit rusty because I haven't used it since this summer, so I had to do a bit of brushing up. I definitely think I over-engineered this solution, but I did it on purpose to show that I understand how data structures can be used to write efficient code, and because I'm really excited to be interviewing at Tesla. 