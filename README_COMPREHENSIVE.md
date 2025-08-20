# MINI Programming Language Compiler

[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)
[![Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Build System](https://img.shields.io/badge/Build-Flex%2FBison-green.svg)](https://www.gnu.org/software/bison/)

## Overview

**MINI** (**M**inimalistic **I**nstruction **I**nterface) is a modern, type-safe programming language designed for educational purposes. This compiler transpiles MINI source code to C, providing a complete learning environment for compiler construction principles.

### Key Features

✅ **Strong Type System**: `number`, `string`, `list<TYPE>`  
✅ **Dynamic Lists**: C++ vector-like functionality with automatic resizing  
✅ **Control Flow**: `when/otherwise`, `repeat`, `begone`  
✅ **String Operations**: Concatenation, escape sequences, type conversion  
✅ **Memory Safety**: Automatic cleanup, no manual memory management  
✅ **Clean Syntax**: Readable and beginner-friendly  
✅ **Educational Focus**: Complete compiler pipeline implementation

## Table of Contents

- [Quick Start](#quick-start)
- [Language Specification](#language-specification)
- [Data Types](#data-types)
- [Operations](#operations)
- [Control Flow](#control-flow)
- [Built-in Functions](#built-in-functions)
- [Examples](#examples)
- [Compiler Architecture](#compiler-architecture)
- [Building the Compiler](#building-the-compiler)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## Quick Start

### Prerequisites

- **Windows** environment (primary target)
- **Flex** (lexical analyzer generator)
- **Bison** (parser generator)
- **GCC** (C compiler)

### Build and Run

1. **Clone the repository**
   ```bash
   git clone https://github.com/ahmed-farhanur-rashid/MINI-Compiler.git
   cd MINI-Compiler
   ```

2. **Build the compiler**
   ```batch
   cd src
   build.bat
   ```

3. **Write your first MINI program** (`hello.mini`)
   ```mini
   --- My first MINI program ---
   string message = "Hello, MINI!";
   display(message);
   
   number answer = 42;
   display("The answer is " + toString(answer));
   ```

4. **Compile and run**
   ```batch
   src\mini.exe hello.mini
   hello.exe
   ```

## Language Specification

### Comments

```mini
--- This is a block comment ---
--- Comments can span
    multiple lines ---
```

### Variable Declaration

```mini
number x = 42.5;          // Number with initialization
string s = "Hello";       // String with initialization
number y;                 // Declaration only
string empty;             // Empty string declaration
```

### Lists (Dynamic Arrays)

```mini
list<number> numbers(5);   // Number list with initial capacity
list<string> words(3);     // String list with initial capacity
```

## Data Types

### Numbers
- **Double-precision floating-point**
- **Integer literals**: `42`, `-17`, `0`
- **Decimal literals**: `3.14159`, `-2.718`
- **Scientific notation**: `6.02e23`, `-1.602e-19`

### Strings
- **UTF-8 compatible text**
- **Escape sequences**: `\n`, `\t`, `\r`, `\\`, `\"`
- **Dynamic memory management**
- **Automatic concatenation**

```mini
string text = "Line 1\nLine 2\tTabbed";
string quoted = "He said \"Hello!\"";
string path = "C:\\Users\\Name\\file.txt";
```

### Lists
- **Dynamic arrays** with automatic resizing
- **Type-safe**: `list<number>` or `list<string>`
- **C++ vector-like interface**

```mini
list<number> scores(10);
scores.push_back(95.5);
scores.push_back(87.2);
display(scores.size());    // Outputs: 2
display(scores[0]);        // Outputs: 95.5
```

## Operations

### Arithmetic Operations
```mini
number a = 10 + 5;         // Addition: 15
number b = 20 - 8;         // Subtraction: 12
number c = 6 * 7;          // Multiplication: 42
number d = 15 / 3;         // Division: 5
number e = (a + b) * c;    // Parentheses for precedence
```

### String Operations
```mini
string greeting = "Hello" + " " + "World!";
string result = "Value: " + toString(42) + " units";
string combined = greeting + "\n" + result;
```

### Comparison Operations
```mini
when (x > y) { display("x is greater"); }
when (name == "Alice") { display("Hello Alice!"); }
when (score >= 90 and score <= 100) { display("Grade A"); }
```

### List Operations
```mini
list<number> data(5);
data.push_back(10);        // Add element
data.push_back(20);
number size = data.size(); // Get size
number first = data[0];    // Access by index
data.pop_back();           // Remove last element
```

## Control Flow

### Conditional Statements

```mini
when (condition) {
    // Execute if condition is true
}

when (condition) {
    // Execute if condition is true
} otherwise {
    // Execute if condition is false
}
```

### Loops

```mini
repeat (5) {
    display("Hello!");
}

number count = 3;
repeat (count) {
    display("Counting down...");
}
```

### Break Statement

```mini
repeat (10) {
    when (some_condition) {
        begone;  // Break out of loop
    }
    display("Continuing...");
}
```

## Built-in Functions

### Type Conversion
```mini
number pi = 3.14159;
string pi_text = toString(pi);     // "3.14159"
number parsed = toNumber("42.5");  // 42.5
```

### Input/Output
```mini
display("Enter your name: ");
string name;
fetch(name);                       // Read from stdin
display("Hello " + name + "!");

number age;
display("Enter your age: ");
fetch(age);                        // Read number from stdin
```

### List Methods
```mini
list<string> names(2);
names.push_back("Alice");          // Add element
names.push_back("Bob");
display(names.size());             // Get count: 2
string first = names[0];           // Access: "Alice"
names.pop_back();                  // Remove last
```

## Examples

### Basic Program
```mini
--- Basic arithmetic and output ---
number x = 10;
number y = 20;
number sum = x + y;
display("Sum: " + toString(sum));
```

### String Manipulation
```mini
--- String operations ---
string first = "Hello";
string second = "World";
string greeting = first + ", " + second + "!";
display(greeting);

string escaped = "Line 1\nLine 2\tTabbed content";
display(escaped);
```

### List Processing
```mini
--- Dynamic list example ---
list<number> scores(3);
scores.push_back(85.5);
scores.push_back(92.0);
scores.push_back(78.5);

number total = 0;
number i = 0;
repeat(scores.size()) {
    total = total + scores[i];
    i = i + 1;
}

number average = total / scores.size();
display("Average score: " + toString(average));
```

### Control Flow Example
```mini
--- Grade calculator ---
number score = 87;

when (score >= 90) {
    display("Grade: A");
} otherwise {
    when (score >= 80) {
        display("Grade: B");
    } otherwise {
        when (score >= 70) {
            display("Grade: C");
        } otherwise {
            display("Grade: F");
        }
    }
}
```

## Compiler Architecture

### Frontend Pipeline
1. **Lexical Analysis** (`lexer.l`)
   - Tokenizes source code using Flex
   - Handles keywords, operators, literals, identifiers
   - Processes escape sequences and comments

2. **Syntax Analysis** (`parser.y`)
   - Parses tokens using Bison
   - Builds Abstract Syntax Tree (AST)
   - Enforces grammar rules and precedence

3. **Semantic Analysis** (`symbol_table.c`)
   - Variable declaration tracking
   - Type checking and validation
   - Scope management

### Backend Pipeline
4. **Code Generation** (`codegen.c`)
   - Transpiles AST to C code
   - Implements memory management
   - Optimizes string operations

5. **Compilation** (GCC)
   - Compiles generated C code
   - Links with standard libraries
   - Produces executable binary

### Memory Management
- **Automatic cleanup** system
- **Pointer registration** for dynamic allocations
- **No manual memory management** required
- **Garbage collection** on program exit

## Building the Compiler

### Windows (Primary Platform)
```batch
cd src
build.bat
```

The build script performs:
1. `bison -d parser.y` - Generate parser
2. `flex lexer.l` - Generate lexer
3. `gcc parser.tab.c lex.yy.c symbol_table.c ast.c codegen.c main.c -o mini.exe` - Compile

### Manual Build
```bash
# Generate parser
bison -d parser.y

# Generate lexer
flex lexer.l

# Compile all components
gcc parser.tab.c lex.yy.c symbol_table.c ast.c codegen.c main.c -o mini
```

## Testing

### Test Suite
The project includes comprehensive tests in the `tests/` directory:

- **Basic Tests**: Variable declarations, assignments
- **String Tests**: Concatenation, escape sequences
- **List Tests**: Dynamic arrays, methods
- **Control Flow Tests**: Conditionals, loops
- **Stress Tests**: Complex expressions, memory management

### Running Tests
```batch
cd examples
compile-all.bat
```

### Individual Test
```batch
src\mini.exe tests\comprehensive_test.mini
tests\comprehensive_test.exe
```

### Test Categories

1. **Functionality Tests**
   - `basic_test.mini` - Core language features
   - `string_concat.mini` - String operations
   - `list_test.mini` - Dynamic arrays
   - `when_otherwise.mini` - Conditionals

2. **Edge Case Tests**
   - `precision_test.mini` - Floating-point accuracy
   - `escape_test.mini` - Escape sequences
   - `edge_cases.mini` - Boundary conditions

3. **Stress Tests**
   - `ultimate_stress.mini` - All features combined
   - `comprehensive_test.mini` - Full language coverage

## Project Structure

```
MINI-Compiler/
├── src/                    # Source code
│   ├── main.c             # Compiler entry point
│   ├── lexer.l            # Flex lexer specification
│   ├── parser.y           # Bison parser grammar
│   ├── ast.c/h            # Abstract Syntax Tree
│   ├── symbol_table.c/h   # Symbol table management
│   ├── codegen.c/h        # Code generation
│   ├── build.bat          # Build script
│   └── mini.exe           # Compiled compiler
├── tests/                 # Test suite
│   ├── basic_test.mini    # Basic functionality
│   ├── list_test.mini     # List operations
│   ├── comprehensive_test.mini  # Full coverage
│   └── ...                # Additional tests
├── examples/              # Example programs
│   ├── test_01.mini       # Simple examples
│   ├── compile-all.bat    # Batch compilation
│   └── ...                # More examples
├── README.md              # Original README
├── PRECISION_STUDY.md     # Technical documentation
└── LICENSE                # License file
```

## Technical Details

### AST Node Types
- **Literals**: `AST_NUMBER`, `AST_STRING`
- **Variables**: `AST_VAR`, `AST_DECL`, `AST_ASSIGN`
- **Operations**: `AST_ADD`, `AST_SUB`, `AST_MUL`, `AST_DIV`
- **Comparisons**: `AST_EQ`, `AST_NEQ`, `AST_LT`, `AST_GT`
- **Control Flow**: `AST_WHEN`, `AST_REPEAT`, `AST_BREAK`
- **Lists**: `AST_LIST_DECL`, `AST_LIST_ACCESS`, `AST_LIST_FUNC`
- **I/O**: `AST_DISPLAY`, `AST_FETCH`
- **Functions**: `AST_TOSTRING`, `AST_TONUMBER`

### Symbol Table
- **Variable tracking** with type information
- **Scope management** for nested blocks
- **Type checking** during compilation
- **Error reporting** for undeclared variables

### Code Generation Features
- **C transpilation** with readable output
- **String optimization** for concatenation
- **Memory management** with automatic cleanup
- **Type-safe** list implementations

## Contributing

This project is designed for educational purposes. Contributions that enhance the learning experience are welcome:

1. **Bug fixes** in the compiler implementation
2. **Additional test cases** for edge conditions
3. **Documentation improvements**
4. **Example programs** demonstrating features
5. **Educational materials** explaining concepts

### Development Guidelines
- Maintain clean, readable code
- Add comprehensive test cases
- Update documentation for new features
- Follow existing code style conventions

## License

**Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)**

### Permissions
- ✅ **Personal use** - Use for learning and education
- ✅ **Educational use** - Use in academic settings
- ✅ **Research use** - Use for research projects
- ✅ **Modification** - Create derivative works
- ✅ **Distribution** - Share with attribution

### Restrictions
- ❌ **Commercial use** - Requires explicit written permission
- 📋 **Attribution required** - Must credit original author
- 📋 **Share-alike** - Derivatives must use same license
- ⚠️ **No warranty** - Use at your own risk

### Commercial Licensing
For commercial use, contact: ahmed.farhanur.rashid@example.com

---

## Acknowledgments

This compiler was developed as part of a Compiler Design course, demonstrating practical implementation of:

- **Lexical analysis** with Flex
- **Syntax analysis** with Bison
- **Semantic analysis** with symbol tables
- **Code generation** with C transpilation
- **Memory management** strategies
- **Type system** implementation

The MINI language serves as an excellent introduction to compiler construction principles while providing a usable programming environment for simple applications.

---

**Educational use encouraged! 🎓**

*Build your understanding of compilers with MINI!*
