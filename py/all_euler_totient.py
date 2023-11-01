
def all_euler_totient(n):
    sieve = [i for i in range(n+1)]
    for i in range(2, n+1):
        if sieve[i] == i:
            for j in range(i, n+1, i):
                sieve[j] -= sieve[j] // i
    return sieve
