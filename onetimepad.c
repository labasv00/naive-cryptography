#include <stddef.h>
#include <stdio.h>

char xor(char m, char k) {
    if (m == k) {
        return '0';
    } else {
        return '1';
    }
}

void xor_strings(char* m, char* k, char* c, size_t length) {
    for (int i = 0; i < length; i++) {
        c[i] = xor(m[i], k[i]);
    }
}

void print_binary(char* m, size_t length) {
    for (int i = 0; i < length - 1; i++) {
        printf("%c", m[i]);
        if ((i + 1) % 4 == 0){
            printf(" ");
        }
    }

    printf("\n");

}

int main() {
    char key[] = "010001010110000110010100";
    char plaintext[] = "010011000100000101010110";
    char ciphertext[sizeof(key)];

    if (sizeof(key) != sizeof(plaintext)){
        printf("The size of the key and the plaintext is different\n");
        return -1;
    }

    xor_strings(plaintext, key, ciphertext, sizeof(key));
    print_binary(plaintext, sizeof(plaintext));
    print_binary(key, sizeof(key));
    printf("=\n");
    print_binary(ciphertext, sizeof(ciphertext));


}
