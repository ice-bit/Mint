function fib(n) {
    if(n <= 2) return n;

    return fib(n - 2) + fib(n - 1);
}


for(let i = 0; i < 20; i = i + 1)
    print(fib(i));