# gscunit

[![CodeFactor](https://img.shields.io/codefactor/grade/github/Iswenzz/gscunit?label=codefactor&logo=codefactor)](https://www.codefactor.io/repository/github/iswenzz/gscunit)
[![License](https://img.shields.io/github/license/Iswenzz/gscunit?color=blue&logo=gitbook&logoColor=white)](https://github.com/Iswenzz/gscunit/blob/master/LICENSE)

![](https://i.imgur.com/qLLn0vR.png)

A testing library for GSC, as well as a CLI to run CoD4 binary and run unit tests.

## Instructions
Install [gsclib](https://github.com/Iswenzz/gsclib) and it's pre-requisites and copy the libs directory to your mod folder.

## Command Line Interface
The CLI can be installed with [NPM](https://www.npmjs.com/).
```
npm install -g gscunit
```
```
Options:
      --version   Show version number                                  [boolean]
  -l, --location  The CoD4 path location                                [string]
  -t, --target    The CoD4 binary target name							[string]
  -a, --args      The CoD4 arguments                                    [string]
  -p, --profile   The gscunit profile path                              [string]
      --help      Show help                                            [boolean]
```

## Example
```c
#include libs\gscunit\_main;
#include libs\gscunit\_utils;

runTests()
{
    gscunitEnv();

	suite("Example");
	it(::test_HelloWorld, "HelloWorld");

	summarize(true);
}

test_HelloWorld()
{
	EXPECT_EQ(1 + 1, 2);
}
```
More advanced examples on the [gsclib](https://github.com/Iswenzz/gsclib/tree/master/libs/gsclib/__test__) library.

## Profile
gscunit settings can be set with a profile file (.ini)
```ini
[CLI]
location=
target=
args=
```

## [Download](https://github.com/Iswenzz/gscunit/releases)

## Contributors:
***Note:*** If you would like to contribute to this repository, feel free to send a pull request, and I will review your code. Also feel free to post about any problems that may arise in the issues section of the repository.
