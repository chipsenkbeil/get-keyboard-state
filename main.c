#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define MAX_DEVICES 32 /* Maximum number of devices marked as event# */
#define SET_DEVICE_PATH(INDEX,STR) sprintf(STR, "/dev/input/event%d", INDEX)

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

/*
 * Opens a device.
 *
 * index: The index of the device to open
 *
 * Return: The file descriptor or -1 if failed
 */
int open_device(int index)
{
    int fd; {
        char path[256];
        SET_DEVICE_PATH(index, path);

        if ((fd = open(path, O_RDONLY | O_NONBLOCK)) < 0)
            fprintf(stderr, "Cannot open %s: %s.\n", 
                path, strerror(errno));
    }

    return fd;
}

/*
 *
 */
int event_occurred(int f, struct input_event* event)
{
    ssize_t n;
    n = read(f, event, sizeof *event);

    // Ignore potential errors and pass false as return
    if (n == (ssize_t) -1) return 0;
    if (n != sizeof *event) { return 0; }

    return 1;
}

/*
 *
 */
int event_is_keyboard(struct input_event* event)
{
    return (event->type == EV_KEY && event->value >= 0 && event->value <= 2);
}

// ============================================================================
// = MAIN PROGRAM
// ============================================================================

int main(int argc, char** argv)
{
    struct input_event ev;
    int fd;

    if ((fd = open_device(2)) < 0)
        return EXIT_FAILURE;

    for(;;) {
        if (event_occurred(fd, &ev) && event_is_keyboard(&ev)) {
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
        }
    }

    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));

    return EXIT_FAILURE;
}

