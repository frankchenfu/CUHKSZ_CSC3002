/*
 * Project: CSC3002 Assignment 6
 * File: main.cpp
 * Last modified: 2023/12/04
 * Author: Chen Fu
 * Student ID: 122090020
 * 
 * Author's development environment (suggested running environment):
 *  OS: Windows 11 Home 22H2 22621.2715 (64-bit)
 *  Visual Studio: Microsoft Visual Studio Community 2022 (64-bit) Version 17.8.2
 *  OpenCV: 4.8.1 (Windows release binaries)
 *  Compiler: MSVC v142 (Visual Studio 2019 C++ x64/x86 build tools)
 *  C++ standard: ISO C++14
 *  Extra compiler options: (None)
 *  Extra include directories: <opencv_build_dir>/install/include;
 * 							   <opencv_build_dir>/install/include/opencv2
 *  Extra library directories: <opencv_build_dir>/install/x64/vc16/lib
 *  Additional dependencies: opencv_world481d.lib
 * Note that the program runs directly in the debug mode, no release version is provided.
 * 
 * Description:
 * 	This file contains all the functions of the project.
 *  It will allow users to input some images and give the prediction results.
 * 
 *  There is an option "pre_train" which is asked at the beginning of the program.
 *  If the user chooses "no", the program will train a new model as follows:
 * 		It loads data from MNIST dataset, trains and tests a KNN model.
 * 		The model is saved to "mnist_knn.xml".
 *  Finally, the program will load the model and give the prediction results.
 * 
 * Expected output (expected running time: around 4 mins):
 * 	$ Loading train data...	Done.
 * 	$ 60000 train data have been loaded.
 * 	$ Loading test data...	Done.
 * 	$ 10000 test data have been loaded.
 * 	$ Training KNN model...	Done.
 * 	$ Testing KNN model...	Done.
 * 	$ Accuracy: 96.61%
 * 	$ KNN model has been saved to "mnist_knn.xml".
 * 
 * Notes on the output:
 *  Note 1: the accuracy may be slightly different due to
 * 			the random selection of training data.
 *  Note 2: Due to some minor differences on the opencv-core-parallels
 * 			model/library, there may be some warnings like:
 * 
 * 				[ INFO:0@2.359] global registry_parallel.impl.hpp:96
 * 				...
 * 				=> Failed
 * 			But it is not an error nor a warning, and the program will
 * 			run normally. It will not affect the accuracy of the model.
 */

#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>

#define assertm(exp, msg) assert(((void)msg, exp))

cv::Mat train_images, train_labels;
cv::Mat test_images, test_labels;
cv::Ptr<cv::ml::KNearest> knn;

/*
 * MNIST Dataset Description:
 * Training set: 60000 images, 28x28 pixels, 0-9 digits.
 * Test set: 10000 images, 28x28 pixels, 0-9 digits.
 * 
 * TRAINING SET IMAGE FILE (train-images-idx3-ubyte):
 * [offset] [type]          [value]          [description] 
 * 0000     32 bit integer  0x00000803(2051) magic number
 * 0004     32 bit integer  60000            number of images 
 * 0008     32 bit integer  28               number of rows 
 * 0012     32 bit integer  28               number of columns
 * 0016     unsigned byte   ??               pixel
 * 0017     unsigned byte   ??               pixel
 * ........
 * xxxx     unsigned byte   ??               pixel
 * Pixels are organized row-wise. Pixel values are 0 to 255.
 * 0 means background (white), 255 means foreground (black).
 * 
 * TRAINING SET LABEL FILE (train-labels-idx1-ubyte):
 * [offset] [type]          [value]          [description]
 * 0000     32 bit integer  0x00000801(2049) magic number (MSB first)
 * 0004     32 bit integer  60000            number of items
 * 0008     unsigned byte   ??               label
 * 0009     unsigned byte   ??               label
 * ........
 * xxxx     unsigned byte   ??               label
 * The labels values are 0 to 9.
 * 
 * TEST SET IMAGE FILE (t10k-images-idx3-ubyte):
 * [offset] [type]          [value]          [description]
 * 0000     32 bit integer  0x00000803(2051) magic number
 * 0004     32 bit integer  10000            number of images
 * 0008     32 bit integer  28               number of rows
 * 0012     32 bit integer  28               number of columns
 * 0016     unsigned byte   ??               pixel
 * 0017     unsigned byte   ??               pixel
 * ........
 * xxxx     unsigned byte   ??               pixel
 * Pixels are organized row-wise. Pixel values are 0 to 255.
 * 0 means background (white), 255 means foreground (black).
 * 
 * TEST SET LABEL FILE (t10k-labels-idx1-ubyte):
 * [offset] [type]          [value]          [description]
 * 0000     32 bit integer  0x00000801(2049) magic number (MSB first)
 * 0004     32 bit integer  10000            number of items
 * 0008     unsigned byte   ??               label
 * 0009     unsigned byte   ??               label
 * ........
 * xxxx     unsigned byte   ??               label
 * The labels values are 0 to 9.
 */


// Load data from MNIST dataset.

/*
 * Read data of exact size from file.
 * Directly supported types: uint32_t, uint8_t, char.
 * Any other types should be read by _readn().
 * 
 * Note that the data is stored in big-endian format,
 * i.e. any data with more than 1 byte should be reversed.
 */
inline uint32_t _byteswap32(uint32_t v){
	return ((v & 0xff000000) >> 24) | 
		   ((v & 0x00ff0000) >>  8) | 
		   ((v & 0x0000ff00) <<  8) | 
		   ((v & 0x000000ff) << 24);
}
inline void _read32(std::ifstream& fin, uint32_t& v){
	fin.read(reinterpret_cast<char*>(&v), sizeof(v));
	v = _byteswap32(v);
}
inline void _read8(std::ifstream& fin, uint8_t& v){
	fin.read(reinterpret_cast<char*>(&v), sizeof(v));
}
inline void _read8(std::ifstream& fin, char& v){
	fin.read(reinterpret_cast<char*>(&v), sizeof(v));
}
inline void _readn(std::ifstream& fin, uint8_t* v, uint32_t n){
	fin.read(reinterpret_cast<char*>(v), n);
}
/*
 * Load data from MNIST dataset.
 * @param filename: the path of the file to be loaded.
 * @param stat: if true, the data is images, otherwise the data is labels.
 * @param images, labels: the loaded data.
 */
void load_data(const char* imagefile, const char* labelfile,
			   cv::Mat& images, cv::Mat& labels, const char* typ){
	printf("$ Loading %s data...\t", typ);

	std::ifstream fimg(imagefile, std::ios::binary);
	std::ifstream flab(labelfile, std::ios::binary);
	assertm(fimg.is_open(), "Cannot open image file.");
	assertm(flab.is_open(), "Cannot open label file.");

	uint32_t magic_img, magic_lab;
	uint32_t n, n_img, n_lab;
	uint32_t rows, cols;
	_read32(fimg, magic_img);
	_read32(fimg, n_img);
	_read32(fimg, rows);
	_read32(fimg, cols);
	_read32(flab, magic_lab);
	_read32(flab, n_lab);
	assertm(magic_img == 0x00000803, "Incorrect image file.");
	assertm(magic_lab == 0x00000801, "Incorrect label file.");
	assertm(n_img == n_lab, "The number of images and labels are not equal.");
	assertm(rows == 28 && cols == 28, "The size of images is not 28x28.");
	n = n_img;

	images = cv::Mat(n, rows*cols, CV_32FC1);
	labels = cv::Mat(n, 1, CV_32SC1);
	uint8_t *img = new uint8_t[rows*cols];
	uint8_t label;
	for(uint32_t i=0; i<n; i++){
		_readn(fimg, img, rows*cols);
		_read8(flab, label);
		for (uint32_t j = 0; j < rows * cols; j++)
			images.at<float>(i, j) = static_cast<float>(img[j]) / 255.0f;
		labels.at<int32_t>(i, 0) = static_cast<int32_t>(label);
	}
	delete[] img;
	fimg.close();
	flab.close();

	printf("Done.\n$ %d %s data have been loaded.\n", n, typ);
}

// Train and test a KNN model.
void train_knn(cv::Ptr<cv::ml::KNearest>& knn,
			   const cv::Mat& train_images, const cv::Mat& train_labels){
	printf("$ Training KNN model...\t");
	knn = cv::ml::KNearest::create();
	knn->setDefaultK(12);
	knn->setIsClassifier(true);
	knn->train(train_images, cv::ml::ROW_SAMPLE, train_labels);
	printf("Done.\n");
}
void test_knn(const cv::Ptr<cv::ml::KNearest>& knn,
			  const cv::Mat& test_images, const cv::Mat& test_labels){
	printf("$ Testing KNN model...\t");

	cv::Mat pred;
	knn->predict(test_images, pred);
	pred.convertTo(pred, CV_32SC1);
	uint32_t n = test_images.rows, tot = 0;
	for(uint32_t i=0; i<n; i++)
		if(pred.at<int32_t>(i, 0) == test_labels.at<int32_t>(i, 0))
			tot++;
	printf("Done.\n$ Accuracy: %.2f%%\n", static_cast<float>(tot) / n * 100);
}

int main() {
	// Welcome message and guidelines to the user.
	printf("Welcome! This is the assignment 6 of CSC3002 by Chen Fu (122090020).\n");
	printf("This program implements a KNN model to recognize handwritten digits.\n");
	printf("The MNIST dataset is used to train and test the model.\n\n");
	printf("A pre-trained model is provided. You can use it directly or train a new one.\n");
	printf("Otherwise you may need about 4 mins to train a new model.\n");
	printf("Use the pre-trained model? ([Y]/N) ");
	std::string pre_train;
	std::getline(std::cin, pre_train);
	while(pre_train != "Y" && pre_train != "y" && pre_train != "N" && pre_train != "n" && pre_train != ""){
		printf("Use the pre-trained model? ([Y]/N) ");
		std::getline(std::cin, pre_train);
	}

	if(pre_train == "N" || pre_train == "n"){
		// Load data from MNIST dataset.
		std::string train_img_path = "train-images-idx3-ubyte/train-images-idx3-ubyte";
		std::string train_lab_path = "train-labels-idx1-ubyte/train-labels-idx1-ubyte";
		std::string test_img_path = "t10k-images-idx3-ubyte/t10k-images-idx3-ubyte";
		std::string test_lab_path = "t10k-labels-idx1-ubyte/t10k-labels-idx1-ubyte";
		load_data(train_img_path.c_str(), train_lab_path.c_str(), train_images, train_labels, "train");
		load_data(test_img_path.c_str(), test_lab_path.c_str(), test_images, test_labels, "test");

		// Train KNN
		train_knn(knn, train_images, train_labels);

		// Test KNN
		test_knn(knn, test_images, test_labels);

		// Save KNN model
		knn->save("mnist_knn.xml");
		printf("$ KNN model has been saved to \"mnist_knn.xml\".\n");
	}

	knn = cv::ml::StatModel::load<cv::ml::KNearest>("mnist_knn.xml");
	while(true){
		printf("\nInput the path of the image to be recognized (or \"exit\" to exit): ");
		std::string path;
		std::getline(std::cin, path);
		if(path == "exit")
			break;
		cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
		if(img.empty()){
			printf("Cannot open the image.\n");
			continue;
		}
		cv::resize(img, img, cv::Size(28, 28));
		img.convertTo(img, CV_32FC1);
		img = img.reshape(1, 1) / 255.0f;
		float pred = knn->predict(img);
		printf("Predicted label: %.0f\n", pred);
	}
	return 0;
}