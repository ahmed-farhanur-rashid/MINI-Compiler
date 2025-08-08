# MINI-Compiler

A compiler for the MINI programming language - a modern, type-safe language with dynamic lists and clean syntax.

**MINI** stands for **M**inimalistic **I**nstruction **I**nterface.

## Features

✅ **Strong Type System**: `number`, `string`, `list<TYPE>`  
✅ **Dynamic Lists**: C++ vector-like functionality with automatic resizing  
✅ **Control Flow**: `when/otherwise`, `repeat`, `begone`  
✅ **String Operations**: Concatenation, escape sequences, type conversion  
✅ **Memory Safety**: Automatic cleanup, no manual memory management  
✅ **Clean Syntax**: Readable and beginner-friendly  

## License

**Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)**

- ✅ **Free for personal, educational, and research use**
- ✅ **Open source with attribution requirements**
- ❌ **Commercial use requires explicit written permission**
- 📋 **Derivative works must use same license and document changes**
- ⚠️ **No warranty - use at your own risk**

For commercial licensing, contact: [Your Email]

## Quick Example

```mini
--- MINI Language Example ---
list<number> numbers(3);
numbers.push_back(10);
numbers.push_back(20);
numbers.push_back(30);

string msg = "Total elements: ";
display(msg);
display(numbers.size());

number i = 0;
repeat(numbers.size()) {
    display(numbers[i]);
    i = i + 1;
}
```

## Getting Started

1. **Build the compiler**: Run `src/build.bat`
2. **Write MINI code**: Create `.mini` files in `tests/` directory
3. **Compile**: `src/mini.exe your_file.mini`
4. **Run**: `./your_file.exe`

See `tests/` directory for comprehensive examples.

---

*Part of Compiler Design coursework - Educational use encouraged!*
