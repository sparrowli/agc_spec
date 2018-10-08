# Simulate the AGC

```
$ cd spec/release
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ ./agc_spec
```


<h3 id=git-submodule>git submodule</h3>
```oh-my-zsh
% git submodule add ${url.git} sub/path/module
% git submodule init
% git submodule update
% git add .gitmodule
% git commit
```


```oh-my-zsh
% git clone --single-branch AGCSPEC ${url.git}
% git submodule update --init --recursive
% mkdir gtest_spec/release && cd gtest_spec
# sometimes cmake cannot find openssl
# might add openssl root directory to include_directories() in CMakeLists.txt
% export LDFLAGS="/usr/local/opt/openssl/lib"
% export CPPFLAGS="/usr/local/opt/openssl/include"
% cmake .. && make
% ./test_spec
```
