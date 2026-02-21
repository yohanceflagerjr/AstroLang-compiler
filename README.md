# AstroLang — A Custom Space-Themed Programming Language

A project I worked on in college implementing a compiler and stack-based virtual machine for a custom programming language with space-themed syntax. Built in C++.

> **Status:** Partially complete(project was cut short due to time constraints) — core features work but some functionality is unfinished. See [What Works](#what-works) below.

---

## About the Language

AST is a statically-typed, C-like language with a space-exploration theme. Keywords replace standard C/C++ syntax to create a unique feel while maintaining familiar control flow structures.

### Keyword Mapping

| AST Keyword | Equivalent |
|---|---|
| `radius` | `int` |
| `mass` | `float` |
| `signal` | `bool` |
| `message` | `string` |
| `frag` | `char` |
| `ping` / `scramble` | `true` / `false` |
| `Controlroom` | `main()` |
| `launchfunction` | function declaration |
| `blackhole` | `void` |
| `orbit` | `for` |
| `galaxy` | `while` |
| `activate` / `ignite` | `do` / `while` |
| `probe` | `if` |
| `investigate` | `else if` |
| `fallback` | `else` |
| `reenter` | `return` |
| `halt` | `break` |
| `eternal` | `const` |
| `cluster` | array |

### Operators

| AST Symbol | Equivalent |
|---|---|
| `_` | `=` (assignment) |
| `__` | `==` (equality) |
| `@` | `+` |
| `@@` | `++` |
| `^` | `-` |
| `^^` | `--` |
| `#` | `*` |
| `%` | `/` |
| `*` | `%` (modulo) |
| `\|` | `>` |
| `-` | `<` |
| `++` | `&&` |
| `?` | `\|\|` |
| `~` | `!` |
| `...` | `;` |
| `[ ]` | `{ }` (code blocks) |
| `{ }` | `( )` (parentheses) |
| `::text::` | `"text"` (string literal) |
| `;x;` | `'x'` (char literal) |
| `!` | `//` (single-line comment) |
| `///` ... `\\\` | `/* */` (multi-line comment) |

---

## Example Program

```
!This is a comment

!A void function
launchfunction blackhole House{}
[
    radius Counter _ 5 ...
    message Sentence _ ::TESTING PLEASE WORK:: ...
    signal SystemReady _ ping ...

    !for loop
    orbit {radius Index _ 0... Index - 6... Index@@}
    [
        mass Earth _ 20 ...
    ]
]

!Main function - execution starts here
radius Controlroom{}
[
    radius Counter _ 0 ...

    !while loop
    galaxy {Counter -_ 3 ++ Counter - 3}
    [
        Counter @@ ...
    ]

    !do-while loop
    activate
    [
        radius Test _ 3 ...
        signal SignalStrength _ ping ...
    ] ignite {SignalStrength __ scramble} ...

    !if / else if / else
    probe {Counter |_ 5 ? Counter | 5}
    [
        mass Earth _ 20 ...
    ]
    investigate {Counter __ 5 ++ ~SignalStrength}
    [
        mass Moon _ 3 ...
    ]
    fallback
    [
        mass Moon _ 0 ...
    ]

    reenter 0 ...
]
```

---

## Architecture

The project is made up of four main components:

**Scanner (`scanner.h` / `scanner.cpp`)** — Tokenizes the source file character by character, handling all AST operators, literals, and keywords. Returns tokens with a name and lexeme.

**Parser (`parser.h`)** — A recursive descent parser that walks the token stream and drives code generation. Handles expressions, declarations, control flow, and function definitions.

**Symbol Table (`symbol_table.h` / `symbol_table.cpp`)** — Tracks declared variables and functions with their type, kind, scope, and memory address. Supports scoped lookup.

**Interpreter (`interpreterWorking.cpp`)** — A stack-based virtual machine with a 64KB memory space and a rich instruction set (~100 opcodes) covering arithmetic, logic, branching, memory load/store, and I/O.

The compiler emits bytecode into a memory array and writes it to a `.x` file, which the interpreter then loads and executes.

---

## What Works

- Variable declarations for `int`, `float`, `bool`, `string`
- Integer and float arithmetic (`+`, `-`, `*`, `/`, `%`)
- Assignment and expressions
- `if` / `else if` / `else` statements
- `while` loop
- `do-while` loop
- `for` loop (basic)
- Function declarations
- Symbol table with scoping
- Code generation targeting the stack-based VM
- Bytecode written to `.x` file and loadable by interpreter

## Known Limitations / Not Yet Implemented

- `char` literals have a code generation bug (noted in `parsePrimaryExpr`)
- Output (`transmit`) and input (`scan`) statements are parsed but code generation is incomplete
- Function calls from within expressions are not fully implemented
- `missionControl` / `catchall` (switch/case equivalent) is parsed but not code-generated
- Some relational operators emit no code (comparison result not pushed to stack)
- The symbol table uses flat array indexing as addresses rather than actual memory offsets

---

## Building

```bash
g++ -o compiler main.cpp scanner.cpp symbol_table.cpp
```

Requires a `keywords.txt` file in the working directory (included in repo) and an input `.AST` source file named `example.AST`.

To run the interpreter separately:
```bash
g++ -o interpreter interpreterWorking.cpp
./interpreter
# Enter filename at prompt (without .x extension)
```

---

## License

This project is provided as-is for educational and portfolio purposes.