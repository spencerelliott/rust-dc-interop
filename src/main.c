#include <stdio.h>

#ifdef DREAMCAST
#include <kos.h>

KOS_INIT_FLAGS(INIT_DEFAULT);

#if USE_ROMDISK
    extern uint8 romdisk[];
    KOS_INIT_ROMDISK(romdisk);
#endif

#endif

extern char rust_main();

int main(int argc, const char** argv) {
    char rust_response = rust_main();

    printf("Rust responded with: %d!\n", rust_response);

    return 0;
}