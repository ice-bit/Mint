let sum = 0;

for(let i = 0; i < 1000; i = i + 1) {
    if((i % 3 == 0) || (i % 5 == 0)) sum = sum + i;
}

print "The sum of all the multiples of 3 or 5 below 1000 is:";
print sum;
