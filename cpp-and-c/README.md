# Compile

```bash
$> make all
g++ -c lib.cpp -o libtest.a -static
gcc -o static main.c libtest.a -lstdc++
g++ -c lib.cpp -o libtest.so -fPIC -shared
gcc -o dynamic main.c -ltest -lstdc++ -L. -Wl,-rpath=.

$> ./static 
hello from c++! =)
1 + 2 = 3
```
