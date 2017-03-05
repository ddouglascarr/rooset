Rooset
========

Rooset is a democratic structured decision system.

![Circle CI](https://circleci.com/gh/ddouglascarr/rooset.png?style=shield&circle-token=df883778b8696aba14f9b77e350c90a587702517)

Dependencies
=============

A Linux environment with the following:
  - Postgresql  
  - [EventStore](https://geteventstore.com) 3.9 +
  - Clang 3.8 +
  - Java 8 +
  - NodeJS 6.9 +
  - CMake 3.0 +
  - bash


Install
=========

```
  $ git clone https://github.com/ddouglascarr/rooset
  $ cd rooset
  $ git submodule update -- init
  $ ./configure
```

Tests
=========

```
  $ ./run-all-tests
```
