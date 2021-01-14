/*
*	File:	README.txt
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

IMPORTANT NOTE:
	THE PROGRAM DOES NOT WORK PROPERLY!

COMPILATION:

	- Open a terminal in the current directory and type the following:
		- mkdir build
		- cd build
		- cmake ..
		- make

=================================================================================================

TRAINING [1]:
	./train -ncells=5x5 -uLBP -img_norm=1 -v -class=0 -knn_K=1 -@dataset_path=../../fsiv_gtscb -@model=../../models/knn_1.xml

	./train -ncells=5x5 -uLBP -img_norm=1 -t -class=0 -knn_K=1 -@dataset_path=../../fsiv_gtscb -@model=../../models/knn_1.xml

EXECUTION [1]:

	./test -ncells=5x5 -img_norm=1 -uLBP -class=0 -@dataset=../../fsiv_gtscb -@model=../../models/knn_1.xml -@image=../../data/signal_1.jpeg

=================================================================================================

TRAINING [2]:
	./train -ncells=3x3 -uLBP -img_norm=0 -v -class=1 -svm_C=1.0 -svm_K=0 -svm_D=3.0 -svm_G=1.0 -@dataset_path=../../fsiv_gtscb -@model=../../models/svg_1.xml

	./train -ncells=3x3 -uLBP -img_norm=0 -t -class=1 -svm_C=1.0 -svm_K=0 -svm_D=3.0 -svm_G=1.0 -@dataset_path=../../fsiv_gtscb -@model=../../models/svg_1.xml

EXECUTION [2]:

	./test -ncells=3x3 -img_norm=0 -uLBP -class=1 -@dataset=../../fsiv_gtscb -@model=../../models/svg_1.xml -@image=../../data/signal_1.jpeg

=================================================================================================

TRAINING [4]:
	./train -ncells=3x3 -uLBP -img_norm=0 -v -class=0 -knn_K=1 -@dataset_path=../../fsiv_gtscb -@model=../../models/knn_2.xml

	./train -ncells=3x3 -uLBP -img_norm=0 -t -class=0 -knn_K=1 -@dataset_path=../../fsiv_gtscb -@model=../../models/knn_2.xml

EXECUTION [4]:

	./test -ncells=3x3 -img_norm=0 -uLBP -class=0 -@dataset=../../fsiv_gtscb -@model=../../models/knn_2.xml -@image=../../data/signal_1.jpeg
