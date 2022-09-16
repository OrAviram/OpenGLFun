# OpenGLFun
I don't remember when, but some time ago I wanted to make a small OpenGL project just to learn general graphics concepts.
I followed TheCherno's OpenGL series and also used a series of lectures on computer grpahics (by Justin Solomon).

The way it's structured is that "Tools" are wrappers for OpenGL, and "Tests" is small scenes to test concepts. I have no idea what the commented out stuff in Main.cpp is, it was there and didn't run properly, so I just commented it out to put here.
I'm pretty unhappy with the events system, everything about it is bad, I just copied it from Cherno's game engine series, but every time I wanted to add events I realized how cumbersome it is. Whatever, it's there. The tests system is pretty cool, though.

The tests are divided into "BasicTests" and "PolynomialTests". The first is just generally testing the OpenGL wrappers. Most of them suck. The one cool test is the one I used for testing tessellation shaders, which is just...it's that, it's not interesting, but making the tessellation levels animate just looks cool.
The polynomial tests come from first trying to implement the cubic bezier curves from the graphics lectures. Then I decided they aren't going enough in depth, so I found a math oriented book related to it (Tom Lyche and Knut Morken's "Spile Methods" thing from the University of Oslo), and the rest of the tests are just straight up implementing the things from the book (which was very interesting also theory-wise and definitely answered all questions I had from the lectures).
The last thing, "graphs test", was an attempt at making a small Desmos-like thing. I worked on it for a day, got a grid going (and I think the points buffer is related), and gave up because I was bored. I wanted it to implement even more things related to splines, and generally making math related tests, because it was fun. But, it won't happen.

This is a project I for sure won't come back to, I just wanted to put it out there and make a small document. It was fun, I learned a lot, and I'll probably one day make a similar project with a better events system (and also better wrapper since I'm unhappy with the ones here). Maybe even in D to make CodeMyst proud.
