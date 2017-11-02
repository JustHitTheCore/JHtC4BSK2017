#!/usr/bin/env python

from random import randint, randrange
import subprocess, json
from hashlib import sha256

old_bin = bin
def bin(a):
    return  old_bin(a)[2:]

def eh(a):
    try:
        return a.encode('hex')
    except:
        return hex(a)[2:].strip('L')

def dh(a):
    a = a.strip()
    return ('0'*(len(a)%2) + a).decode('hex')

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def gcd(a,b,k,seed):
    p=b+1
    while b:
        a, b = b, a%b
    if int(bin(p)[:k/4+32],2) < seed:
        return a
    else:
        return 2

def random_nbit_integer(n):
    return randint(2**(n-1), 2**n-1)

def pi(b, a):
    return pow(a,17,b)

def is_prime(p):
    if p % 2 == 0:
        return False
    s = 0
    d = p-1
    while True:
        quotient, remainder = divmod(d, 2)
        if remainder == 1:
            break
        s += 1
        d = quotient
    
    def try_composite(a):
        if pow(a, d, p) == 1:
            return False
        for i in range(s):
            if pow(a, 2**i * d, p) == p-1:
                return False
        return True
    
    for i in range(6):
        a = randrange(2, p)
        if try_composite(a):
            return False
    return True

def fast_multiply(p, qv, k, seed):
    e = 65537
    nv = p*qv
    t = bin(nv)[:k/8]
    u = bin(pi(seed, int(bin(p)[:k/4+32],2)))
    u = '0'*(k/4+32 - len(u)) + u
    l = bin(nv)[-(5*k)/8 + 32:]
    n = int(t + u + l, 2)
    q = n/p + 1 - ((n/p)%2)
    while gcd(e, q-1, k, float('inf')) > 1 or not is_prime(q):
        m = random_nbit_integer(k/8-40)
        m += (1+(m%2)) / 2
        q = q^m
        n = p*q
    return n

def random_nbit_prime(n):
    while True:
        p = random_nbit_integer(n)
        if is_prime(p):
            return p    

class SecureKey:
    def __init__(self):
        self.params = {}

    def importKey(self, filename):
        self.params = json.loads(open(filename).read())

    def encrypt(self, plain):
        if 'e' not in self.params or 'n' not in self.params:
            raise ValueError("import or generate key first")
        plain = int(eh(plain), 16)
        if plain >= self.params['n']:
            raise ValueError("Too large text")
        cip = pow(plain, self.params['e'], self.params['n'])
        return dh(eh(cip))

    def decrypt(self, cip):
        if 'd' not in self.params or 'n' not in self.params:
            raise ValueError("import or generate key first")
        cip = int(eh(cip), 16)
        if cip >= self.params['n']:
            raise ValueError("Too large text")
        plain = pow(cip, self.params['d'], self.params['n'])
        return dh(eh(plain))

    def sign(self, text):
        text = sha256(text).digest()
        return self.decrypt(text)

    def verify(self, text, signature):
        if 'e' not in self.params or 'n' not in self.params:
            raise ValueError("import or generate key first")
        text = sha256(text).digest()
        text = int(eh(text), 16)
        signature = int(eh(signature), 16)
        if pow(signature, self.params['e'], self.params['n']) == text:
            return True
        return False

    def generate(self, e=65537, size=1024):
        if type(e) not in [int, long]:
            raise ValueError("Bad e")
        if e < 3:
            raise ValueError("Too small e")
        if size != 1024:
            raise ValueError("Only 1024 size is implemented at the moment")
        k = size
        seed = 476283116406539741845175463956657874046958850596520333086272652099928678076182181180321
        while True:
            p = random_nbit_prime(k/2)
            if gcd(e, p-1, k, seed) == 1:
                break
        q = random_nbit_prime(k/2)
        n = fast_multiply(p,q,k,seed)
        d = modinv(e, (p-1)*(n/p-1))
        self.params['e'] = e
        self.params['n'] = n
        self.params['p'] = p
        self.params['q'] = n/p
        self.params['d'] = d

    def exportKey(self, filename):
        data = {}
        if 'e' not in self.params or 'n' not in self.params:
            raise ValueError("import or generate params first")
        data['e'] = self.params['e']
        data['n'] = self.params['n']
        if 'd' in self.params and 'p' in self.params and 'q' in self.params:
            data['p'] = self.params['p']
            data['q'] = self.params['q']
            data['d'] = self.params['d']
        with open(filename, 'w') as f:
            f.write(json.dumps(data))

    def getParams(self):
        return self.params['n'], self.params['e'], self.params['d'], self.params['p'], self.params['q']
