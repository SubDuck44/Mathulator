#pragma once

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////

typedef struct {
	char*  str;
	size_t len;
} StringView;

////////////////////////////////////////////////////////////////////////////////

#define CONSTARR_SIZE(arr) sizeof(arr) / sizeof(arr[0])

////////////////////////////////////////////////////////////////////////////////

#define ArrayN(t, n)                                                           \
	typedef struct {                                                           \
		t*     ptr;                                                            \
		size_t len;                                                            \
		size_t cap;                                                            \
	} n

#define Array(t) ArrayN(t, t##s)

/* Adds an element to a dynamic array, enlarging its capacity if needed. If
   capacity is zero, it will allocate memory for 16 elements into ptr. */
#define ArrayAdd(arr, x)                                                       \
	do {                                                                       \
		if((arr).len >= (arr).cap)                                             \
			(arr).ptr = reallocarray(                                          \
				(arr).ptr, ((arr).cap = (arr).cap ? (arr).cap << 1 : 16),      \
				sizeof((arr).ptr[0])                                           \
			);                                                                 \
                                                                               \
		(arr).ptr[(arr).len++] = (x);                                          \
	} while(0)

/* Runs code 'body' for each element in the dynamic array while providing a
   pointer to the current element as an iterator 'it' */
#define ArrayLoop(arr, body) ArrayLoopN(arr, it, body)

/* Same as ArrayLoop but with a custom iterator */
#define ArrayLoopN(arr, it, body)                                              \
	for(size_t i = 0; i < (arr).len; i++) {                                    \
		typeof((arr).ptr[0])* it = &(arr).ptr[i];                              \
		body                                                                   \
	}

/* Loops through an array with a named iterator like ArrayLoopN. Terminates if
   an expression "pred" is true at any iteration and stores the element
   corresponding to that iteration in "result". */
#define ArrayFindN(arr, result, it, pred)                                      \
	typeof((arr).ptr[0])* result = NULL;                                       \
	ArrayLoopN(arr, it, {                                                      \
		if(pred) {                                                             \
			result = it;                                                       \
			break;                                                             \
		}                                                                      \
	})

/* Identital to ArrayFindN but without a named iterator */
#define ArrayFind(arr, result, pred) ArrayFindN(arr, result, it, pred)

#define ArrayLast(arr) (arr).ptr[(arr).len - 1]

#define ArrayFree(arr)                                                         \
	do {                                                                       \
		free((arr).ptr);                                                       \
		(arr).ptr = NULL;                                                      \
		(arr).len = (arr).cap = 0;                                             \
	} while(0)

////////////////////////////////////////////////////////////////////////////////

#define MIN(a, b) ((a) < (b)) ? a : b

#define MAX(a, b) ((a) > (b)) ? a : b

#define CLAMP(min, max, val) MIN(MAX((min), (val)), (max))

////////////////////////////////////////////////////////////////////////////////

ArrayN(char, UserInput);

// Halts thread and stores next user input in 'result' of type 'UserInput'
void get_user_input(UserInput* result);

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

void get_user_input(UserInput* result) {
	result->ptr = NULL;
	result->cap = 0;

	// Signed to retain error value
	ssize_t local_len = getline(&result->ptr, &result->cap, stdin);

	if(local_len <= 0) {
		fputs(
			"\n", // Hack to avoid jumping the error message
			stderr
		);

		err(1, "Failed to read line");
	}

	// Signed value converted to standard size_t
	result->len = (size_t) local_len;
}

#endif
