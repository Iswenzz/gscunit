# gscunit

[![Build status](https://ci.appveyor.com/api/projects/status/wb0m9puc58f5d74w?svg=true)](https://ci.appveyor.com/project/Iswenzz/gscunit)
[![CodeFactor](https://www.codefactor.io/repository/github/iswenzz/gsclib/badge)](https://www.codefactor.io/repository/github/iswenzz/gscunit)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

``gscunit`` is a testing library for GSC as well as a CLI to run CoD4 binary and run tests.

![](https://i.imgur.com/CiKF8pv.png)

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
```

## [Download](https://github.com/Iswenzz/gscunit/releases)

## Contributors:
***Note:*** If you would like to contribute to this repository, feel free to send a pull request, and I will review your code. Also feel free to post about any problems that may arise in the issues section of the repository.
