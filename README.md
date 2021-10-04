# DistanceVerify
##Notes
- This project uses ArUco markers, with opencv_contrib
- Has to be manually built from source. Using the tutorial from https://www.youtube.com/watch?v=_fqpYLM6SCw
	- Latest Cmake version at the time of writing this doc has support for an experimental "Visual Studio 17 2022" version, that isn't released yet.
	- Had to use older Cmake to build OpenCV.
	- Remember to check proper versions.
	- Video does not talk about enabling opencv_world in cmake, remember to do that for yourself

- Getting camera image: https://riptutorial.com/opencv/example/21401/get-image-from-webcam
- Adding text with box: https://answers.opencv.org/question/27695/puttext-with-black-background/
- Actually detecting markers: https://docs.opencv.org/4.5.3/d5/dae/tutorial_aruco_detection.html
	- Scrolling further down into the page gives a complete working example rather than building the entire thing.