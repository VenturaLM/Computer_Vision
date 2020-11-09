- Calibrate the video:
	./calibration -w=9 -h=6 -n=55 -pt=chessboard -o=../intrinsics.yml -op -oe -V ../video.mp4
	
	
- Execution 1:
	./augReal -cols 6 -rows 9 -size 3 -yaml ../intrinsics.yml -video ../video.mp4
	
- Execution 2:
	./augReal -cols 6 -rows 9 -yaml ../intrinsics.yml -video ../video.mp4 -image ../gif.mp4
	./augReal -c 6 -r 9 -y ../intrinsics.yml -v ../video.mp4 -i ../gif.mp4
