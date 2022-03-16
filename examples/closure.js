function make_counter() {
    let count = 0;

    function counter() {
        count = count + 1;
        return count;
    }

    return counter;
}

let i = 0;
let count = make_counter();

while(i < 5) {
    print count();
    i = i + 1;
}
