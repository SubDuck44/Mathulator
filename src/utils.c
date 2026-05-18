#pragma once

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

// Dynamic arrays
#define DEFAULT_DYNARR_CAP 16

#define DynArrN(type, name)                                                    \
	typedef struct {                                                           \
		type*  arr;                                                            \
		size_t len;                                                            \
		size_t cap;                                                            \
	} name

#define DynArr(type) DynArrN(type, type##s)

#define DynArrExtend(array, num)                                               \
	do {                                                                       \
		(array)->len += num;                                                   \
		while((array)->cap < (array)->len) {                                   \
			(array)->cap = MAX((array)->cap << 1, DEFAULT_DYNARR_CAP);         \
		}                                                                      \
		(array)->arr =                                                         \
			reallocarray((array)->arr, (array)->cap, sizeof((array)->arr[0])); \
		if(!((array)->arr)) {                                                  \
			fputs("\nERR: Allocation fucked, try again", stderr);              \
		}                                                                      \
	} while(0)

#define DynArrShrink(array, num)                                               \
	do {                                                                       \
		(array)->len -= num;                                                   \
		while((array)->cap >> 1 >= (array)->len) {                             \
			(array)->cap = (array)->cap >> 1;                                  \
		}                                                                      \
		(array)->arr =                                                         \
			reallocarray((array)->arr, (array)->cap, sizeof((array)->arr[0])); \
		if(!((array)->arr)) {                                                  \
			fputs("\nERR: Allocation fucked, try again", stderr);              \
		}                                                                      \
	} while(0)

#define DynArrPush(array, data)                                                \
	do {                                                                       \
		DynArrExtend(array, 1);                                                \
		(array)->arr[(array)->len - 1] = data;                                 \
	} while(0)

#define DynArrPop(array) DynArrShrink(array, 1);

#define DynArrInsert(array, index, src)                                        \
	do {                                                                       \
		if(index > (array)->len - 1) {                                         \
			(array)->arr[(array)->len - 1] = *src;                             \
		} else {                                                               \
			for(size_t i = (array)->len; i > index; i--) {                     \
				(array)->arr[i] = (array)->arr[i - 1];                         \
			}                                                                  \
			(array)->arr[index] = *src;                                        \
		}                                                                      \
	} while(0)

#define DynArrRemove(array, index)                                             \
	do {                                                                       \
		if(index > (array)->len) {                                             \
			fputs("\nERR: Try providing an actual index next time", stderr);   \
		} else {                                                               \
			if(index != (array)->len - 1) {                                    \
				for(size_t j = 0; j < (array)->len - 2; j++) {                 \
					(array)->arr[j] = (array)->arr[j + 1];                     \
				}                                                              \
			}                                                                  \
			DynArrPop(array);                                                  \
		}                                                                      \
	} while(0)

#define DynArrLoop(array, body)                                                \
	do {                                                                       \
		for(size_t i = 0; i < (array)->len; i++) {                             \
			body                                                               \
		}                                                                      \
	} while(0)

// Misc
#define MIN(a, b) ((a) < (b)) ? a : b
#define MAX(a, b) ((a) > (b)) ? a : b
#define CLAMP(min, max, val) MIN(MAX((min), (val)), (max))
