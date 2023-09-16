#ifndef DEV_H
#define DEV_H

// development + debugging macros
// these only run if DEV_MODE macro is set to true in types.h

/*
    full list of dev mode differences:
        - DevAssert, DevBreakpoint, and DevPrintf run
        - ESC quits the game
        - camera pos is labeled
        - all hitboxes are drawn
*/

#define DevStringify(x) \
    #x

#define DevFmtFile(file) \
    (strrchr((file), '/') ? strrchr((file), '/') + 1 : (file))

#define DevAssert(cond) \
    do { \
        if (DEV_MODE) { \
            if ((cond)) \
                printf("[DEVLOG] Assertion Passed @ %s:%d - `%s`\n", \
                    DevFmtFile(__FILE__), \
                    __LINE__, \
                    DevStringify((cond))); \
            else { \
                printf("[DEVLOG] Assertion Failed @ %s:%d - `%s`, exiting\n", \
                    DevFmtFile(__FILE__), \
                    __LINE__, \
                    DevStringify((cond))); \
                exit(1); \
            } \
        } \
    } while(0)

#define DevBreakpoint \
    if (DEV_MODE) printf("[DEVLOG] Breakpoint @ %s:%d\n", DevFmtFile(__FILE__), \
        __LINE__)

#define DevPrintf(fmt, ...) \
    if (DEV_MODE) printf("[DEVLOG] `" fmt "` @ %s:%d\n", __VA_ARGS__, \
        DevFmtFile(__FILE__), __LINE__)

#endif // DEV_H