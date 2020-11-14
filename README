# Motivation
This is the source code accompanying my master thesis _"Interactive ray tracing of skinned animations"_ ("Interactieve ray tracing van skeletgebasseerde animaties", in dutch). The main goals of this thesis are:
* Exploring various techniques that make ray tracing faster.
* Exploring techniques specifically for ray tracing skinned animations
* Make a thorough comparison of these techniques.
* A working ray tracer implementing these techniques.

Some interesting features of my ray tracer:
* Packet ray tracing: A technique to trace multiple rays through the scene together in "packets". This allows aggregating the trace cost over multiple rays, speeding up rendering.
* BVH (Bounding Volume Hierarchy) acceleration data structure. The implementation allows different methods to (re)build the data structure as to adapt to the dynamically changing geometry of a skinned animation. We support the following techniques:
  * Fast Rebuilds, rebuild the BVH every frame to adapt to the changing geometry. Based on an approximate SAH (Surface Area Heuristic). Implementation based on [On fast Construction of SAH based Bounding Volume Hierarchies](http://www.sci.utah.edu/~wald/Publications/2007/ParallelBVHBuild/fastbuild.pdf) by Ingo Wald
  * Motion decomposition & Fuzzy datastructures, uses 2 level ray tracing with a top level data structure that is rebuild every frame and sub-level data structures that are calculated in advance based on the skinned animation. Implementation based on [Interactive Ray Tracing of Skinned Animations](https://domino.mpi-inf.mpg.de/intranet/ag4/ag4publ.nsf/93832a04987390a3c12567530068622d/d2422b791713dafdc12571b00020f74e/$FILE/IRTSA_preprint.pdf) by J. Gunther et al.
  * Refitting, refit the bounding boxes in the BVH to adapt to the motion. With this technique, the quality of the BVH will degrade over time because the bounding boxes won't fit the geometry tightly anymore over time.
  * A novel technique combining refitting & 2 level ray tracing.

To build this code, you will need Qt, Qmake, Boost, ...? Maybe I will make it build someday with plain Make and without the Qt & Boost dependencies. Maybe not ;)
