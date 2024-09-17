# msvcfilt

**msvcfilt** is a command line utility inspired by the GNU binutils program [c++filt](http://linux.die.net/man/1/c++filt). It reads text from standard input, and prints each line with replacements to standard output. For example, `FOO ?func1@a@@AAEXH@Z BAR` becomes `FOO private: void __thiscall a::func1(int) BAR`.

## This Fork

This fork updated the original `msvcfilt` to C++ 23 and reduced the scope. Its intended purpose is to be used with LLVM's [`llvm-cov`](https://llvm.org/docs/CommandGuide/llvm-cov.html) tool, which accepts a `-Xdemangler` flag. To use `msvcfilt` with `llvm-cov`, pass `-Xdemangler=msvcfilt`:

```shell
llvm-cov show foo.exe --instr-profile foo.profdata --format=html -Xdemangler=msvcfilt > foo.html
```

To be compatible with `llvm-cov`, `msvcfilt` will output "?" on empty lines. Otherwise, `llvm-cov` can error with "error: demangler did not provide expected number of symbols".

## Releases

You can download binaries compiled on GitHub Actions from the [releases tab](https://github.com/Nerixyz/msvcfilt/releases).
