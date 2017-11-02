#!/usr/bin/env python3

from random import randint

print("Calculate a**b mod m")
print("You don't have a lot of time!")

for _ in range(120):
	a=randint(10**100, 10**200)
	b=randint(10**100, 10**200)
	m=randint(10**100, 10**200)

	print("a =", a)
	print("b =", b)
	print("m =", m)
	print("ans:")

	ans = int(input())

	if pow(a, b, m) != ans:
		print("Nope!")
		exit(1)

with open("flag.txt", 'r') as fin:
    print(fin.read())
