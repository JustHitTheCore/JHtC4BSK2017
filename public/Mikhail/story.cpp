#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char * story;
int i;
int pass_len;

void foo() {
    char password[64];
    story = new char[1001];
    
    printf("Tell us your story: ");

    story[0] = '\x90';

    read(0, story+1, 1000);

    printf("Tell us your password: ");
    pass_len = read(0, password, 1000);

    for(i = 0; i < strlen(story); ++i) {
        story[i] ^= password[i%strlen(password)];
    }

    // We will save your story here... someday.

    char const *const msg = "Yay! We encrypted your story! It's secret now at ";
    for(i = 0; i < 49; ++i) {
        password[pass_len + i] = msg[i];
    }

    size_t *const ptr = reinterpret_cast<size_t *>(&password[pass_len + 49]);
    *ptr = reinterpret_cast<size_t>(story);

    printf("%s", password+pass_len);
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("This app to safe story memorizations doesn't work.\n"
    "But it will, so we are sharing what we made so far.\n");

    foo();
}

