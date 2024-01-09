# Background subtraction

C++ implementation of a background subtraction algorithm, following the method "Adaptive background mixture models for real-time tracking" by Stauffer and Grimson.

The paper can be found [here](http://www.ai.mit.edu/projects/vsam/Publications/stauffer_cvpr98_track.pdf).

To use:

-   create a `build` folder
-   `cmake ../`
-   `cmake --build .`
-   `./backgroundSubtraction K lr T`, specifying values for K, lr and T.

