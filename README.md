YAFEL
=====

Linux, OSX: [![Build Status](https://api.travis-ci.org/tjolsen/YAFEL.svg?branch=master)](https://travis-ci.org/tjolsen/YAFEL)

Yet Another Finite Element Library

YAFEL is a general-purpose finite element library written entirely in C++/C++11.
It is being developed primarily as a learning exercise and research code.
The library is entirely self-contained -- it does not link to any non-standard 
external libraries. This decision was made not because I think my hand-rolled data structures
and algorithms will out-perform those under professional development (PETSc, Trilinos, deal.ii),
but because I wanted to fully understand the details of what is occurring at every stage of
the computations. Since this library is primarily for personal use, features are added as
the need for them arises. If you have any good ideas for something to add, please get in
contact with me to discuss them.

I'd like to give credit where it's due: this library was *heavily* inspired by the Deal.II
library (https://github.com/dealii/dealii), which is maintained by Professor Wolfgang Bangerth
at Colorado State University (formerly Texas A&M).
I had the privilege of using his library in a class, and it inspired me to really get my
hands dirty learning what goes on in these codes.
As such, many of the workflows in this library are similar to those in the Deal.II library,
though I have tweaked some features here and there to suit my preferences (in particular,
a general aversion to linking to massive GB-scale libraries to achieve small tasks that
I could do sufficiently well mysef).


Compilation and installation
==========================

Compiling and installing YAFEL is a very simple process. Simply run:

./configure.sh

This script defines an environment variable called YAFELDIR that points to the top-level
directory of the source tree. It also appends the appropriate commands to the .bashrc
file so that you will not have to run this script ever again.

After that, run:

make

At the time of writing, the library is under rapid-enough development that it is not
worth installing into a system-wide directory (eg, /usr/local/).

Using the library
=================

To compile a program using the YAFEL library, simply  include the file 'common.mk' 
in a makefile to have access to the necessary compiler and linker flags. CXXFLAGS and
LDFLAGS are defined in here, so if you wish to add your own, you should use the +=
operator.

Warning
=======

This library is under very active development. Due to this, the API for the various
parts frequently change subtly as I discover better/different ways to do things.
Most of these changes shouldn't break much existing code (eg, mass-convert of 'int'
or 'unsigned' to 'std::size_t' in linear algebra data structures).
