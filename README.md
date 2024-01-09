# Background subtraction

C++ implementation of a background subtraction algorithm, following the method "Adaptive background mixture models for real-time tracking" by Stauffer and Grimson.

The paper can be found [here](http://www.ai.mit.edu/projects/vsam/Publications/stauffer_cvpr98_track.pdf).

This has been tested on WSL (with Ubuntu). It uses OpenMP to process the pixels in parallel.

To use:

-   create a `build` folder
-   `cmake ../`
-   `cmake --build .`
-   `./backgroundSubtraction videoname K lr T`, specifying values for videoname, K, lr and T.

For the test video (that can be found [here](https://github.com/opencv/opencv/blob/4.x/samples/data/vtest.avi)), with the following parameters:

-   `K = 4`,
-   `lr = 0.05`,
-   `T = 0.5`,
    we obtain the following result (fps are set to the original video but implementation might be slower):

    ![demo](demo.gif)
