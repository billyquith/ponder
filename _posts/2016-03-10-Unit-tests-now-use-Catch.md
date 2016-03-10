---
layout: post
title:  "Unit tests now use Catch"
date:   2016-03-10
categories: news tests
---

### Catch

The unit tests have been moved over to [Catch](https://github.com/philsquared/Catch) from Boost
Test. This is a single header unit testing library. It is convenient as there are no external
library dependencies or downloads for users. (And for basic unit testing, why should you need more
than one header?!) 

Additionally, there appeared to be issues with Visual Studio 2015 and Boost, so this should result
in more portability.

### Standalone library

This now means that Boost is no longer required at all to use or test Ponder, and makes Ponder a
standalone library. The Boost comparison code has been left in for now, in case there are future
issues. Boost is a very competent library and if our type traits fail it may be useful to make a
comparison to Boost's behaviour, but this is done on demand and is not part of the regular testing.

