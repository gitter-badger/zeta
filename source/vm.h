#ifndef __VM_H__
#define __VM_H__

#include <stdlib.h>
#include <stdint.h>

/// Heap object pointer
typedef uint8_t* heapptr_t;

/// Value tag (and object header)
typedef uint64_t tag_t;

// FIXME: does it actually make sense to have an object tag (TAG_OBJECT)?
// tags as shape node indices
// do we even care to distinguish between object and non-object?
// object vs non-object tells us if its a reference or not, is this important?

/// Non-object value tags
/// Note: non-object values have the least bit set to zero
/// Note: the boolean false has tag zero
#define TAG_FALSE       0b00000
#define TAG_TRUE        0b00010
#define TAG_INT64       0b00100
#define TAG_FLOAT64     0b00110
#define TAG_STRING      0b01000
#define TAG_ARRAY       0b01010
#define TAG_RAW_PTR     0b01100

/// Object value tags
/// Note: object values have the least bit set to one
#define TAG_OBJECT      0b00001
#define TAG_CLOS        0b00011
#define TAG_AST_CONST   0b00101
#define TAG_AST_REF     0b00111
#define TAG_AST_BINOP   0b01001
#define TAG_AST_UNOP    0b00011
#define TAG_AST_IF      0b01101
#define TAG_AST_CALL    0b01111
#define TAG_AST_FUN     0b10001
#define TAG_RUN_ERR     0b10011

/// Initial VM heap size
#define HEAP_SIZE (1 << 24)

/**
Word value type
*/
typedef union
{
    int64_t int64;

    double float64;

    heapptr_t heapptr;

    tag_t tag;

} word_t;

/*
Tagged value pair type
*/
typedef struct
{
    word_t word;

    tag_t tag;

} value_t;

/// Boolean constant values
const value_t VAL_FALSE;
const value_t VAL_TRUE;

/**
Virtual machine
*/
typedef struct
{
    uint8_t* heapStart;

    uint8_t* heapLimit;

    uint8_t* allocPtr;

    // TODO: global variable slots
    // use naive lookup for now, linear search
    // - big array, names and value words
    // eventually, use global object

} vm_t;

/**
String (heap object)
*/
typedef struct
{
    tag_t tag;

    /// String hash
    uint32_t hash;

    /// String length
    uint32_t len;

    /// Character data, variable length
    char data[];

} string_t;

/**
Array (list) heap object
*/
typedef struct
{
    tag_t tag;

    /// Allocated capacity
    uint32_t cap;

    /// Array length
    uint32_t len;

    /// Array elements, variable length
    /// Note: each value is tagged
    value_t elems[];

} array_t;

value_t value_from_heapptr(heapptr_t v);
value_t value_from_int64(int64_t v);

tag_t get_tag(heapptr_t obj);

/// Initialize the global VM instance
void vm_init();

heapptr_t vm_alloc(uint32_t size, tag_t tag);

string_t* string_alloc(uint32_t len);
void string_print(string_t* str);

array_t* array_alloc(uint32_t cap);
void array_set(array_t* array, uint32_t idx, value_t val);
void array_set_ptr(array_t* array, uint32_t idx, heapptr_t val);
value_t array_get(array_t* array, uint32_t idx);

#endif

