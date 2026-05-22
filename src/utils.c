#pragma once

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

/* Inserts some code into a simple for loop of a dynamic array while providing
   the current element directly via an iterator "it" */
#define ArrayLoop(arr, body) ArrayLoopN(arr, it, body)

/* Same as ArrayLoop but with a named iterator */
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

// Misc

#define MIN(a, b) ((a) < (b)) ? a : b

#define MAX(a, b) ((a) > (b)) ? a : b

#define CLAMP(min, max, val) MIN(MAX((min), (val)), (max))
