// ------------------------------------------
// Make an LED output a name using Morse code
// ------------------------------------------

#include <ctype.h>
#include <string.h>

// Global variables
const char name[] = "Nathan";

const int led1 = D0;     // we'll need to wire an LED to this one to see it blink.
const int led2 = D7;     // permanent little blue LED on the board.

const int di = 1;        // base morse code unit 'dot'.
const int dah = di * 3;  // base morse code unit 'dash' (always 3x a dot).
const int dit = di;      // pseudonym for 'di' when at the end of a letter.

const int base_delay = 250;       // base time unit to flash LED (milliseconds), 1 di.
const int word_delay = base_delay * 7;     // time delay between signalling each word.

// Letter to Morse code mapping
const int morse_code_table[26][5] = {
    { 'A', di, dah,      0, 0 },
    { 'B', dah, di, di, dit   },
    { 'C', dah, di, dah, dit  },
    { 'D', dah, di, dit,    0 },
    { 'E', dit,       0, 0, 0 },
    { 'F', di, di, dah, dit   },
    { 'G', dah, dah, dit,   0 },
    { 'H', di,di, di, dit     },
    { 'I', di, dit,      0, 0 },
    { 'J', di, dah, dah, dah  },
    { 'K', dah, di, dah,    0 },
    { 'L', di, dah, di, dit   },
    { 'M', dah, dah,     0, 0 },
    { 'N', dah, dit,     0, 0 },
    { 'O', dah, dah, dah,   0 },
    { 'P', di, dah, dah, dit  },
    { 'Q', dah, dah, di, dah  },
    { 'R', di, dah, dit,    0 },
    { 'S', di, di, dit,     0 },
    { 'T', dah,       0, 0, 0 },
    { 'U', di, di, dah,     0 },
    { 'V', di, di, di, dah    },
    { 'W', di, dah, dah,    0 },
    { 'X', dah, di, di, dah   },
    { 'Y', dah, di, dah, dah  },
    { 'Z', dah, dah, di, dit  },
};

// write a dot (di or dit)
void output_morse_code(const int interval)
{
    // To blink the LED, first we'll turn it on
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);

    // We'll hold it on for given time interval
    delay(interval * base_delay);

    // Finally we'll turn it off
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

    // Each signal is followed by a single dit delay
    delay(base_delay);
}

// Write a letter in Morse code (i.e. one morse_code_table row)
void output_letter(const int *morse_code_row)
{
    size_t      j;

    // morse_code_row[0] is the letter we are outputting
    
    for (j = 1; j < sizeof(morse_code_table[0][0]); j++) {
        if (morse_code_row[j])
            output_morse_code(morse_code_row[j]);
    }
}

// The setup function runs only once when the device boots up or is reset.
void setup()
{
    // We are going to tell our device that D0 and D7 (led1 and led2 respectively) are going to be output
    // (That means that we will be sending voltage to them, rather than monitoring voltage that comes from them)
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
}

// The loopfunction is the other essential part of a microcontroller program.
// It gets repeated as quickly as possible, forever, after setup is called.
void loop()
{
    size_t i, offset, length = strlen(name);
    int letter;

    for (i = 0; i < length; i++) {
        letter = toupper(name[i]);      // morse code has only upper case

        if (letter < 'A' || letter > 'Z')
            continue;   // sanity checking, do not try to handle bad data

        // find the relevant row in the table for this letter
        offset = letter - 'A'; // 'A' start of sequential rows

        output_letter(morse_code_table[offset]);
    }

    // previous word (name) is complete, let it sink in and then repeat
    delay(word_delay);
}
