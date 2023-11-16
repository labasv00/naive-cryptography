#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char xor(char m, char k) {
    if (m == k) {
        return '0';
    } else {
        return '1';
    }
}

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void num_to_bin(unsigned short int num, char* bin_num) {

    bin_num[4] = '\0';
    
    if (num > 15) {
        return;
    }

    if (num >= 8) {
        bin_num[0] = '1';
        num -= 8;
    } else {
        bin_num[0] = '0';
    }

    if (num >= 4) {
        bin_num[1] = '1';
        num -= 4;
    } else {
        bin_num[1] = '0';
    }

    if (num >= 2) {
        bin_num[2] = '1';
        num -= 2;
    } else {
        bin_num[2] = '0';
    }

    if (num >= 1) {
        bin_num[3] = '1';
        num -= 1;
    } else {
        bin_num[3] = '0';
    }

    if (num != 0) {
        printf("WARN");
    }

}

unsigned short int binary_to_dec(char* bin_num) {
    strrev(bin_num);

    unsigned short int result = 0;
    int pos = 0;
    while (bin_num[pos] != '\0') {
        if (bin_num[pos] == '1'){
            result += pow(2, pos);
        }

        pos += 1;
    }

    return result;
}

void expand_bits(char* normal, char* expanded) {

    expanded[0] = normal[0];
    expanded[1] = normal[2];
    expanded[2] = normal[1];
    expanded[3] = normal[2];
    expanded[4] = normal[3];
    expanded[5] = normal[1];
    expanded[6] = '\0';
}

void xor_strings(char* a, char* b, char* dest, size_t length) {
    for (int i = 0; i < length - 1; i++) {
        dest[i] = xor(a[i], b[i]);
    }
}

void split_bit_stream(char* stream, char* l, char* r) {
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            l[i] = stream[i];
        } else {
            r[i % 4] = stream[i];
        }
    }

    r[4] = '\0'; 
    l[4] = '\0';
}

void join_bit_stream(char* l, char* r, char* dest) {
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            dest[i] = l[i];
        } else {
            dest[i] = r[i % 4];
        }
    }

    dest[8] = '\0';
}

void print_binary(char* m, size_t length) {

    for (int i = 0; i < length - 1; i++) {
        printf("%c", m[i]);
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
    }

    printf("\n");

}

void split_by_row_and_col(char* state, unsigned short int* row, unsigned short int* column) {

    char first_last[] = { state[0], state[5], '\0' };
    char middle[] = { state[1], state[2], state[3], state[4], '\0' };

    *row = binary_to_dec(first_last);
    *column = binary_to_dec(middle);

}

unsigned short int substitution(unsigned short int row, unsigned short int column) {
    unsigned short int* s_table[4];
    unsigned short int row1[] = { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7 };
    unsigned short int row2[] = {  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2 };
    unsigned short int row3[] = {  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8 };
    unsigned short int row4[] = {  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 };
    s_table[0] = row1;
    s_table[1] = row2;
    s_table[2] = row3;
    s_table[3] = row4;

    return s_table[row][column];
}

void round_function(char* key, char* bits, char* result) {
    // 1. Expandimos de 4 a 6
    char expanded[7];
    expand_bits(bits, expanded);

    // 2. Hacemos el XOR con la clave
    char substitution_info[7];
    xor_strings(expanded, key, substitution_info, sizeof(expanded));

    // Obtenemos las coordenadas para la sustitucion
    unsigned short int col; unsigned short int row;
    split_by_row_and_col(substitution_info, &row, &col);

    // Pasamos los resultados por la tabla de sustitucion
    unsigned short int value = substitution(row, col);
    
    num_to_bin(value, result);
}

void cipher_des(char* key, char* plaintext, char* ciphertext) {
    if (strlen(key) != 6) {
        printf("Key size mismatch.\n");
        return;
    }

    if (strlen(plaintext) != 8) {
        printf("Plaintext size mismatch.\n");
        return;
    }

    printf("Plaintext:  "); print_binary(plaintext, 9);

    // Separamos el stream en dos bloques
    char r[5]; char l[5];
    split_bit_stream(plaintext, l, r);

    // Aplicamos la round function sobre la parte derecha
    char round_result[5];
    round_function(key, r, round_result);

    // Y hacemos XOR con 
    char r1[5]; r1[4] = '\0';
    xor_strings(round_result, l, r1, 5);

    char* l1 = r; // Li = Ri-1
    join_bit_stream(l1, r1, ciphertext);
    printf("Ciphertext: "); print_binary(ciphertext, 9);
}

void decipher_des(char* key, char* ciphertext, char* plaintext) {
        if (strlen(key) != 6) {
        printf("Key size mismatch.\n");
        return;
    }

    if (strlen(ciphertext) != 8) {
        printf("Plaintext size mismatch.\n");
        return;
    }

    printf("Ciphertext: "); print_binary(ciphertext, 9);

    // Separamos en dos bloques el ciphertext
    char r[5]; char l[5];
    split_bit_stream(ciphertext, l, r);

    // Aplicamos la round function a la parte izquierda
    char round_result[5];
    round_function(key, l, round_result);

    // Hacemos XOR con R
    char l1[5]; l1[4] = '\0';
    xor_strings(round_result, r, l1, 5);

    char* r1 = l;
    join_bit_stream(l1, r1, plaintext);

    printf("Plaintext:  "); print_binary(plaintext, 9);
}

int main() {
    //char key[] = "011010";
    //char plaintext[] = "01001100";
    char key[] = "011010";
    char plaintext_L[] = "00011010"; // L in binary
    char plaintext_A[] = "01001011"; // A in binary
    // char plaintext_V[] = "01010110"; // V in binary
    
    char ciphertext[sizeof(plaintext_L)];

    char iv[] = "00000000";

    xor_strings(plaintext_L, iv, plaintext_L, 9); // CBC xor with iv
    printf("1:\n"); cipher_des(key, plaintext_L, ciphertext);

    xor_strings(plaintext_A, ciphertext, plaintext_A, 9); // CBC xor with previous
    printf("\n2:\n"); cipher_des(key, plaintext_A, ciphertext);

    // xor_strings(plaintext_V, ciphertext, plaintext_V, 9); // CBC xor with previous
    // printf("\nV:\n"); cipher_des(key, plaintext_V, ciphertext);

    // printf("-- DECIPHER --\n");
    // char plaintext2[sizeof(plaintext_L)];    
    // decipher_des(key, ciphertext, plaintext2);

}
