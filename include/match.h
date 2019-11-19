#ifndef JLIB_MATCH_H
#define JLIB_MATCH_H 

#define jlib_match1(fn, target, value, on_match) do { \
    if (fn(target, value)) \
        on_match \
    else {} \
} while (0)

#define jlib_match2(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match1(fn, target, __VA_ARGS__); \
} while (0)

#define jlib_match3(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match2(fn, target, __VA_ARGS__); \
} while (0)

#define jlib_match4(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match3(fn, target, __VA_ARGS__); \
} while (0)

#define jlib_match5(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match4(fn, target, __VA_ARGS__); \
} while (0)

#define jlib_match6(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match5(fn, target, __VA_ARGS__); \
} while (0)

#define jlib_match7(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match6(fn, target, __VA_ARGS__); \
} while (0)

#define jlib_match8(fn, target, value, on_match, ...) do { \
    if (fn(target, value)) \
        on_match \
    else \
        jlib_match7(fn, target, __VA_ARGS__); \
} while (0)

#define match(count, fn, target, value, on_match, ...) do { \
    if (count == 1) \
        jlib_match1(fn, target, value, on_match); \
    else \
        jlib_match ## count(fn, target, value, on_match, __VA_ARGS__); \
} while (0)

#endif // JLIB_MATCH_H