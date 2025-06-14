#include <stdio.h>

#define KEYSIZE 1024;

struct key {
    int a,b;
};

void RSA_Generator() {

    int p = 2;
    int q = 7;
    int n = p * q;
    int e = 5;
    int phi = (p - 1) * (q - 1);
    
    /*choosing d*/
    int phi_minus_e = phi - e;
    int d = 0;
    int cur_value = phi;
    while(cur_value != 1) {
        cur_value -= phi_minus_e;
        if(cur_value <= 0) {
            cur_value = phi + cur_value;
        }
        d += 1;
    }

    struct key public_key, private_key;
    public_key.a = e;
    private_key.a = d;
    public_key.b, private_key.b = n;

    printf("Public Key: (%d, %d)\n", e, n);
    printf("Private Key: (%d, %d)\n", d, n);
}
