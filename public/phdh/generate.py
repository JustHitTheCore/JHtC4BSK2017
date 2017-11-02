#!/usr/bin/env python
# -*- coding: utf-8 -*-


import os
from random import randint

from Crypto.Util.number import isPrime, getPrime
from hashlib import sha256

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import (
    Cipher, algorithms, modes
)


def encrypt(key, plaintext, associated_data):
    iv = os.urandom(12)

    encryptor = Cipher(
        algorithms.AES(key),
        modes.GCM(iv),
        backend=default_backend()
    ).encryptor()

    encryptor.authenticate_additional_data(associated_data)
    ciphertext = encryptor.update(plaintext) + encryptor.finalize()
    return (iv, ciphertext, encryptor.tag)


def generate_smooth_prime(n_bit_size, g, smooth_bit_size=50):
    while True:
        n = 2
        factors = {2:1}
        while n.bit_length() < n_bit_size - 2*smooth_bit_size:
            q = getPrime(smooth_bit_size)
            n *= q
            if q in factors:
                factors[q] += 1
            else:
                factors[q] = 1

        smooth_bit_size_padded = n_bit_size - n.bit_length()
        while True:
            q = getPrime(smooth_bit_size_padded)
            if isPrime((n*q)+1):
                n = (n*q)+1
                if q in factors:
                    factors[q] += 1
                else:
                    factors[q] = 1
                break
        
        are_primitive_roots = True
        for factor, factor_power in factors.items():
            if pow(g, (n-1)//(factor**factor_power), n) == 1:
                are_primitive_roots = False
                break

        if are_primitive_roots:
            return n, factors

if __name__ == "__main__":
    FLAG = 'JHtC4BSK{...}'

    template = '''
Alice <- p: {} -> Bob
Alice <- g: {} -> Bob
Alice -> g^a (mod p): {} -> Bob
Alice <- g^b (mod p): {} <- Bob

Alice -> aes_gcm_encrypt(key, flag): {} -> Bob
EOT
'''[1:]

    g = 2
    p, p_order_factors = generate_smooth_prime(1024, g, 20)
    a, b = randint(2, p-1), randint(2, p-1)
    A, B = pow(g, a, p), pow(g, b, p)
    
    shared_dh_key = pow(A, b, p)
    assert pow(B, a, p) == shared_dh_key

    key = sha256(str(shared_dh_key)).digest()
    flag_encrypted = encrypt(key, FLAG, '')
    flag_encrypted = map(lambda x: x.encode('hex'), flag_encrypted)

    with open('communication.txt', 'w') as f:
        f.write(template.format(p, g, A, B, flag_encrypted))
