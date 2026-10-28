#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <JHLEDBackpack.h>

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO | ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    character = fgetc(stdin);

    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

void showDashes(HT16K33 *displayMatrix) {
    displayMatrix->clear();

    displayMatrix->writeDigitRaw(0, 0x40);
    displayMatrix->writeDigitRaw(1, 0x40);
    displayMatrix->writeDigitRaw(3, 0x40);
    displayMatrix->writeDigitRaw(4, 0x40);

    displayMatrix->writeDisplay();
    displayMatrix->blinkRate(HT16K33_BLINK_1HZ);

    sleep(4);

    displayMatrix->blinkRate(HT16K33_BLINK_OFF);
}

void showHex(HT16K33 *displayMatrix) {
    displayMatrix->clear();

    displayMatrix->writeDigitNum(0, 10);
    displayMatrix->writeDigitNum(1, 11);
    displayMatrix->writeDigitNum(3, 12);
    displayMatrix->writeDigitNum(4, 13);

    displayMatrix->writeDisplay();

    sleep(3);
}

void countDown(HT16K33 *displayMatrix) {
    printf("Starting Count Down, Engines On\n");

    for (int counter = 10; counter >= 0; counter--) {
        displayMatrix->clear();
        displayMatrix->print(counter);
        displayMatrix->writeDisplay();

        printf("Countdown: %d\n", counter);

        sleep(1);
    }
}

void runClock(HT16K33 *displayMatrix) {
    uint16_t counter = 590;

    printf("Digital clock started at 09:50\n");
    printf("Hit ESC key to exit\n");

    while (1) {
        if (getkey() == 27) {
            break;
        }

        int minutes = counter / 60;
        int seconds = counter % 60;

        displayMatrix->clear();

        displayMatrix->writeDigitNum(0, (minutes / 10) % 10);
        displayMatrix->writeDigitNum(1, minutes % 10);

        displayMatrix->drawColon(true);

        displayMatrix->writeDigitNum(3, seconds / 10);
        displayMatrix->writeDigitNum(4, seconds % 10);

        displayMatrix->writeDisplay();

        printf("Clock: %02d:%02d\n", minutes, seconds);

        sleep(1);

        counter++;

        if (counter >= 6000) {
            counter = 0;
        }
    }
}

int main() {
    HT16K33 *displayMatrix = new HT16K33();

    int err = displayMatrix->openHT16K33();

    if (err < 0) {
        printf("Error: %d\n", displayMatrix->error);
        delete displayMatrix;
        return 1;
    }

    printf(
        "HT16K33 Device Address: 0x%02X\n",
        displayMatrix->kI2CAddress
    );

    printf(
        "HT16K33 Device Bus: 0x%02X\n",
        displayMatrix->kI2CBus
    );

    displayMatrix->begin();
    displayMatrix->clear();

    printf("Task 1: blinking dashes\n");
    showDashes(displayMatrix);

    printf("Task 2: hexadecimal ABCD\n");
    showHex(displayMatrix);

    printf("Task 3: countdown from 10 to 0\n");
    countDown(displayMatrix);

    printf("Task 4: digital clock\n");
    runClock(displayMatrix);

    displayMatrix->clear();
    displayMatrix->writeDisplay();

    displayMatrix->end();
    displayMatrix->closeHT16K33();

    delete displayMatrix;

    return 0;
}

