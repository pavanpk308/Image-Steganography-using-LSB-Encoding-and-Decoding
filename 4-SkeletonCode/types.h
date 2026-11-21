
#ifndef TYPES_H             // Header to prevent multiple inclusions
#define TYPES_H             // Define the header macro

/* User defined types */

typedef unsigned int uint;      // Define uint as an alias for unsigned int

/* Status will be used in fn. return type */

// Define Status enum for function return types
typedef enum
{
    e_success,                        // Value for successful operation
    e_failure                         // Value for failed operation
} Status;

// Define OperationType enum for operation types
typedef enum
{
    e_encode,                      // Value for encode operation
    e_decode,                      // Value for decode operation
    e_unsupported                  // Value for unsupported operation
} OperationType;

#endif
