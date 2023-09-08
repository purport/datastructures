fd .profraw .build/ -x llvm-profdata merge -sparse {} -o {.}.profdata
fd .profdata .build/ -x llvm-cov show  --ignore-filename-regex="include/check.h" --ignore-filename-regex="tests/.*.c" \
    -format=html {.} -instr-profile={} -output-dir={.}-cov

