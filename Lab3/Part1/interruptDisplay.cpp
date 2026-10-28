#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <JHLEDBackpack.h>

extern "C" {
#include "jetsonGPIO.h"
}

static volatile sig_atomic_t keepRunning = 1;

void stopProgram(int) {
    keepRunning = 0;
}

int main() {
    const jetsonGPIO button = gpio397;

    signal(SIGINT, stopProgram);

    HT16K33 display;

    if (!display.openHT16K33()) {
        printf(
            "Unable to open HT16K33. Error: %d\n",
            display.error
        );

        return 1;
    }

    display.begin();
    display.clear();
    display.print(0);
    display.writeDisplay();

    if (gpioExport(button) < 0) {
        return 1;
    }

    usleep(200000);

    gpioSetDirection(button, inputPin);
    gpioSetEdge(button, (char *)"rising");

    int fd = gpioOpen(button);

    if (fd < 0) {
        gpioUnexport(button);
        return 1;
    }

    char value;

    lseek(fd, 0, SEEK_SET);
    read(fd, &value, 1);

    struct pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLPRI | POLLERR;
    pfd.revents = 0;

    int count = 0;

    printf(
        "Waiting for button presses on GPIO397. "
        "Press Ctrl+C to stop.\n"
    );

    while (keepRunning) {
        int result = poll(&pfd, 1, 500);

        if (
            result > 0 &&
            (pfd.revents & (POLLPRI | POLLERR))
        ) {
            lseek(fd, 0, SEEK_SET);
            read(fd, &value, 1);

            count++;

            if (count > 9999) {
                count = 0;
            }

            display.clear();
            display.print(count);
            display.writeDisplay();

            printf(
                "Button press detected. "
                "Display value: %d\n",
                count
            );

            usleep(200000);
        }
    }

    gpioClose(fd);
    gpioUnexport(button);

    display.clear();
    display.writeDisplay();

    display.end();
    display.closeHT16K33();

    return 0;
}
