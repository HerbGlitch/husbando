# Introduction
This document outlines the style for any C code that is to be stored within this repository. This document will most likely be changed over time as the style becomes more set.

# The Basic Tenets of This Style
    - Readability is the main goal of following this style
    - Consistancy should never be compromised
    - When creating memory, said memory should be freed within the same scope
    - If the last tenet is broken there should be a function for freeing memory and a note is to be made in documentation that memory needs to be freed after the function call
    - Doxygen documentation should be applied to every header with the express goal of explaining the headers contents

# Code Layout
## Indentation
Use 4 spaces for indentation, the only exception to this rule is for multi line comments

Closing braces over multiple lines should match up with the initial declaration like:
```c
DOC_Type example = {
    value1,
    value2
};
```

## Maximum Number of Indents
As of now there is no maximum number of indentation, though the suggestion is avoid indentation as much as possible because in herbglitch's opinion it makes the code way less readable

## Spacing
There is no space between text and open or closing parenthesis. The only time to add spaces to a curly brace is when on one line for spacing of contents to make it more readable. Example:
```c
//function declaration
void DOC_Type_ExampleFunction(){
    uint8_t exampleCheck = 1;
    if(exampleCheck == 1){
        printf("Here\n");
    }

    //curly braces with space
    DOC_Type example = { value1, value2 };
}
```

## Trailing Space
As of now trailing spaces are not allowed, please message herbglitch if you find a reason to use trailing spaces

## Maximum Line Length
As of now there is no maximum line length

## Blank Lines
As of now there should never be more than two blank lines next to each other

## Organizing lines
This is up to the programmer to decide what is most readable. The suggestion is to group lines together that relate to each other. Example:
```c
void DOC_Type_ExampleFunction(){
    //the first grouping
    printf("Hello World!");
    printf("\n");

    //the next grouping
    printf("Goodbye World!");
    printf("\n");
}
```

## Inline functions
Do not use inline functions, always indent functions even if they are short. Example:
```c
//do not do this
uint32_t *DOC_Type_ExampleFunction(DOC_Type *type){ return type->data; }

//do this
uint32_t *DOC_Type_ExampleFunction(DOC_Type *type){
    return type->data;
}
```

## Inline if, for, while, etc... Statements
Same rule as the function, also if, for, while, etc.. statements always need to have the curly braces to avoid confusion. Example:
```c
//do not do this. this could cause confusions if the next statement is accidentally indented
if(exampleCheck == 1)
    exampleData = 'c';
examplePath = "temp/path.txt";

//do not do this
if(exampleCheck == 1){ exampleData = 'c'; }

//do this
if(exampleCheck == 1){
    exampleData = 'c';
}
```

## Includes
Every library used in a header or c file should be included in the respective file. The include should be alphabetical in order with local files (ordered by relation) on top and library files below. If in a c file, the matching header should be separate from the other headers. Example:
```c
#include "matchingHeader.h"

#include "../../veryFarBackHeader.h"
#include "../backHeader.h"
#include "closeHeader.h"
#include "otherHeader.h"
#include "folder/forwardHeader.h"
#include "folder/nextFolder/veryFarForwardHeader.h"
#include <stdio.h>
#include <stdlib.h>
```

## Alignment of Pointer Asterisks or Ampersand
Always align an Asterisk and Ampersand with the variable and not the type. Aligning with the type can lead to confusion if multiple variables were created on the same line. Example:
```c
//do not do this, only a is created as a pointer, but it looks like b and c are also pointers
uint32_t* a, b, c;

//this is more clear
uint32_t *a, b, c;
```

## When to Pass a Value as a Pointer
The only time to pass a value as a pointer is if:
    - It cannot be copied
    - It is being modified

# Header Layout
## Include Guards
Include guards should match the file name. It is also recommended to have your namespace and path as part of the include guard to avoid confliction. Example:
```c
// archeus/include/arc/std/vector.h
#ifndef ARC_STD_VECTOR_H_
#define ARC_STD_VECTOR_H_

// header contents here

#endif // !ARC_STD_VECTOR_H_
```

## Boilerplate
The recommendation is after the header guards to wrap the header content with a c++ check so that the code can be used in a c++ project as well. Example:
```c
//include guard

#ifdef __cplusplus
extern "C" {
#endif

// header contents here

#ifdef __cplusplus
}
#endif

//end include guard
```

# Comments
Document all headers with doxygen commenting, there probably shouldn't be any other comment in the header.
For the c file, the recommendation is to break up blocks with comments describing what a block does. It is up to the programmer though because there are times when adding a comment would only be redundant if the code describes what it does. herbglitch likes to think of comments in the c file as like markdown headers, basically easy ways to see what parts of the code do what, which makes it easier to jump back into the code

# Naming Conventions
Before getting into naming conventions it is important to note that this style uses namespaces in all caps separated by an underscore. The recommendation is to have the namespace be as short as possible so like:
```
DOC_
ARC_
TBYTE_
NAMESPACE_
```

## Types
Every type should be upper camel case with the namespace before the type name, like:
```c
typedef struct DOC_Type DOC_Type;
typedef struct DOC_LongerType DOC_LongerType;
```

## Defines and Constants
Defines and constants should be in all caps with a namespace and as descriptive as possible, like:
```c
#define DOC_MEMORY_BLOCK_SIZE 6

const uint8_t DOC_LOBY_MAX_ENTITIES 12;
```

## Functions
Every function should be in upper camel case and have the main type it is modifying (the first parameter) as part of the function name separated by an underscore, like:
```c
void DOC_Type_Example(){
    //function contents
}

int32_t DOC_Type_GetValue(){
    return 0;
}
```

## Callbacks
Callbacks are the same as functions but end with Fn, like:
```c
typedef void (* DOC_Type_CallbackFn)();

typedef DOC_Bool (* DOC_Type_PollDataFn)(void *userdata);
```

## Variables
Variables are lower camel case and should be as self documenting as possible, do not use acronyms (type out the full thing). Also using one letter is usually not recommended (x, y, and in for loops i can sometimes be ok). Remember readability is crucial.

# Herb's Suggestions
I'd suggest avoiding ternary operators, else, and else if statements as much as possible. Usually returning instead of else statements makes the code more readable in my opinion. Also ternary operators almost always can be made more readable if put into an if statement.
