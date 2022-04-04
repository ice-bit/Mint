function fact(n) {
    if(n == 0) return 1;

    return fact(n - 1) * n;
}

print fact(50); // 120
