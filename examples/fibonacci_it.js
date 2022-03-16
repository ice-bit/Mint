let a = 0;
let b = 1;
let tmp = a + b;

for(let i = 0; i < 20; i = i + 1) {
    print tmp;
    a = b;
    b = tmp;
    tmp = a + b;
}
