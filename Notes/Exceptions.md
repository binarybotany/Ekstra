# Exceptions

In modern C++, in most scenarios, the preffered way to report and handle both 
logic errors and runtime errors is to use exceptions. It's especially true when 
the stack might contain several function calls between the function that detects
the error and the function that has the context to handle the error. Exceptions 
provide a formal, well-defined way for code that detects errors to pass the 
information up the call stack.

## Use exceptions for exceptional code

Program errors are divided into two categories: 

* _Logic errors_ caused by programming mistakes. (e.g. index out of range)
* _Runtime errors_ beyond the control of the programmer. (e.g. file not found)

C-style and COM programming report errors using status codes. COM programming
uses the `HRESULT` return value to communicate errors to the caller.

Exceptions are preferred in modern C++ for the following reasons: 

* An exception forces the calling code to recognize an error condition and 
  handle it. Unhandled exceptions stop program execution.
* An exception jumps to the point in the call stack that can handle the error.
  Intermediate functions can let the error propagate. They don't have to 
  coordinate with other layers.
* The exception stack-unwinding mechanism destroys all objects in scope after an
  exception is thrown, according well-defined rules.
* An exception enables a clean separation between the code that detects the 
  error and the code that handles the error.

In the `try` block, if an exception is _thrown_ it will be _caught_ by the first
associated `catch` block whose type matches that of the exception. Execution 
jumps from the `throw` statement to the `catch` statement. If no usable catch 
block is found, `std::terminate` is invoked and the program exits. It is 
recommended to inherit custom exceptions from the type `std::exception`.

C++ doesn't provide a `finally` block to make sure all resources are released if
an exception occurs, the resource acquisition is initialization (RAII) idiom, 
which uses smart pointers, provides the required functionality for resource 
cleanup.

## Basic guidelines

* Use asserts to check for errors that shouldn't ever occur. Use exceptions to
  check for errors that might occur. For example, errors in input validation on
  parameters of public functions.
* Use exceptions when the code that handles the error is separated from the code
  that detects the error by one or more intervening calls. Consider whether to 
  use error codes in performance-critical loops.
* For every function that might throw or propagate an exception, provide one of 
  the three exception guarantees: strong guarantee, basic guarantee, or 
  `noexcept` guarantee.
* Throw exceptions by value, catch them by reference.
* Don't use exception specifications, which are deprecated in C++ 11.
* Use standard library exception types when they apply. Derive custom exceptions
  from the exception class hierarchy.
* Don't allow exceptions to escape from destructors or memory-deallocation 
  functions.

## Exceptions and performance

The exception mechanism has a minimal performance cost if no exception is 
thrown. If an exception is thrown, the cost of the stack traversal and unwinding
is roughly comparable to the cost of a function call. The adverse effect of 
exceptions on performance is likely to be significant only on memory-constrained
systems. Or, in performance-critical loops, where an error is likely to occur 
regularly and there's tight coupling between the code to handle it and the code
reports it.

## Exceptions vs. assertations

Use `assert` statements to test for conditions during development that should 
never be true if all your code is correct. There's no point in handling such an 
error by using an exception, because the error indicates that something in the 
code has to be fixed. 

An `assert` stops execution at the statement so that you can inspect the program
state in the debugger. And exception continues execution from the first 
appropriate catch handler. Use exceptions to check error conditions that might 
occur at runtime even if your code is correct, e.g. "file not found", "out of 
memory".

## Exception specifications and `noexcept`

Exception specifications were introduced in C++ as a way to specify the 
exceptions a function might throw. However, exception specifications proved 
problematic in practice, and are deprecated in the C++ 11 draft standard. Don't
use `throw` exception specifications except for `throw()`, which indicates that
the functions allows no exceptions to escape. `noexcept` is the preferred 
alternative to `throw()`.

# Exceptions and stack unwinding

In the C++ exception mechanism, control moves from the `throw` statement to the
first `catch` statement that can handle the thrown type. When the `catch` 
statement is reached, all of the automatic variables that are in the scope 
between the `throw` and the `catch` statements are destroyed in a process known 
as _stack unwinding_.

1. Control reaches the `try` statement. The guarded section in the `try` block
  is executed.
2. If no exception is thrown, the `catch` block that follows is not executed.
  Execution continues after the last statement of the last `catch` block.
3. If an exception is thrown within the guarded `try` block, and exception 
  object is created from the object that is created by the `throw` operand. This
  implies that a copy constructor may be involved. The compiler looks for a 
  `catch` that can handle that particular exception, in order of their 
  appearance.
4. If no matching `catch` handler is found, `std::terminate` will be invoked.
5. If a matching `catch` handler is found and it catches by value, its formal
  parameter is initialized by copying the exception object. If it catches by 
  reference, the parameter is initialized to refer to the exception object. 
  After the formal parameter is initialized, the process of unwinding the stack 
  begins. This involves the destruction of all automatic objects between the 
  beginning of the `try` block associated with the `catch` handler and the throw
  site of the exception. Destruction happens in reverse order of construction.
  The program resumes execution after the last handler.

# How to design for exception safety 

Functions that are called between the try statement and the throw statement are
not required to know anything about the exception that is thrown. However, they
have to be designed so that they can go out of scope "unexpectedly" at any point
where an exception might propagate up from below, and do so without leaving 
behind partially created objects, leaked memory, or data structures that are in
unusable state.

## Basic techniques

A robust exception-handling policy requires careful thought and should be part 
of the design process. In general, most exceptions are detected and thrown at 
the lower layers of a software module, but typically these layers do not have 
enough context to handle the error or expose a message to end users.

A function should catch and "swallow" an exception only if it is able to 
completely recover from it. In many cases, the correct behavior in the middle 
layers is to let an exception propagate up the call stack. Even at the highest
layer, it might be appropriate to let an unhandled exception terminate a program
if the exception leaves the program in a state in which its correctness cannot 
be guaranteed.