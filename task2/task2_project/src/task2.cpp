#include <grad.h>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <cassert>
#include <iostream>
#include <cmath>

#include "classifier.h"
#include "EasyBMP.h"
#include "linear.h"
#include "argvparser.h"
#include "matrix.h"
#include "grad.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::make_pair;
using std::cout;
using std::cerr;
using std::endl;
using std::pow;
using std::atan2;
using std::array;

using CommandLineProcessing::ArgvParser;

typedef vector<pair<BMP*, int> > TDataSet;
typedef vector<pair<string, int> > TFileList;
typedef vector<pair<vector<float>, int> > TFeatures;

// Load list of files and its labels from 'data_file' and
// stores it in 'file_list'
void LoadFileList(const string& data_file, TFileList* file_list) {
    ifstream stream(data_file.c_str());

    string filename;
    int label;
    
    int char_idx = data_file.size() - 1;
    for (; char_idx >= 0; --char_idx)
        if (data_file[char_idx] == '/' || data_file[char_idx] == '\\')
            break;
    string data_path = data_file.substr(0,char_idx+1);
    
    while(!stream.eof() && !stream.fail()) {
        stream >> filename >> label;
        if (filename.size())
            file_list->push_back(make_pair(data_path + filename, label));
    }

    stream.close();
}

// Load images by list of files 'file_list' and store them in 'data_set'
void LoadImages(const TFileList& file_list, TDataSet* data_set) {
    for (size_t img_idx = 0; img_idx < file_list.size(); ++img_idx) {
            // Create image
        BMP* image = new BMP();
            // Read image from file
        image->ReadFromFile(file_list[img_idx].first.c_str());
            // Add image and it's label to dataset
        data_set->push_back(make_pair(image, file_list[img_idx].second));
    }
}

// Save result of prediction to file
void SavePredictions(const TFileList& file_list,
                     const TLabels& labels, 
                     const string& prediction_file) {
        // Check that list of files and list of labels has equal size 
    assert(file_list.size() == labels.size());
        // Open 'prediction_file' for writing
    ofstream stream(prediction_file.c_str());

        // Write file names and labels to stream
    for (size_t image_idx = 0; image_idx < file_list.size(); ++image_idx)
        stream << file_list[image_idx].first << " " << labels[image_idx] << endl;
    stream.close();
}

// Exatract features from dataset.
// You should implement this function by yourself =)
void ExtractFeatures(const TDataSet& data_set, TFeatures* features)
{
    for (size_t image_idx = 0; image_idx < data_set.size(); ++image_idx)
    {
    	vector<float> one_image_features;
    	int label = data_set[image_idx].second;
    	BMP *simg = data_set[image_idx].first;

    	int size=66;//ширина и высота ресайзнутого изображения
    	int const naps=8; //число напрвалений
    	int psize=2;//размер блоков
    	int n=(size-2)/psize;//Число блоков

    	int sh=simg->TellHeight(), sw=simg->TellWidth();
    	Matrix<float> syark(sh,sw);//попробовать double

    	for(int i=0; i<sh; ++i)
    		for(int j=0; j<sw; ++j)
    			syark(i,j)=0.299*simg->GetPixel(j,i).Red+0.587*simg->GetPixel(j,i).Green+0.114*simg->GetPixel(j,i).Blue;

    	//надо привести все изображения к одному виду (или)
		//1. сжать до sizeхsize --------- chosen!
		//2. расширить до квадратности, потом сжать до НхН
		//3. обрезать до sizeхsize
      	//в data все изображения имеют высоту 64(просто к сведению)

    	Matrix<float> yark(size,size);//яркость квадратизированная
		yark=resize<float>(syark, size, size);//собсно квадратизация
		int h=size, w=size;//(совместимости с до этого написанным кодом)
    	//svertka


    	Matrix<float> gradx(h-2,w-2), grady(h-2,w-2), grad(h-2,w-2);//без краёв
    	Matrix<int> napr(h-2,w-2);

    	//вычисление градиента
    	gradx=sobel_x<float>(yark).submatrix(1,1,h-2,w-2);
    	grady=sobel_y<float>(yark).submatrix(1,1,h-2,w-2);
    	for(int i=0; i<h-2; ++i)
			for(int j=0; j<w-2; ++j)
				grad(i,j)=pow(pow(gradx(i,j),2)+pow(grady(i,j),2),0.5);

    	//вычисление направлений
    	double pi=3.1415926535;
		for(int i=0;i<h-2;++i)
			for(int j=0;j<w-2;++j)
			{
				float temp =atan2(grady(i,j),gradx(i,j));
				temp +=pi;//t in [0, 2pi]
				temp = temp*naps/(pi*2);//t in [0, naps]
				int nap = static_cast<int>(temp+0.5);//в итого направо это 4, вниз -2, влево 0 и 8
				if(nap==naps)
					nap=0;
				napr(i,j)=nap;
			}

		//инициализация блоков
		Matrix<array<float,naps>> blocks(n,n);
		for(int i = 0; i<n; ++i)
			for(int j=0; j< n; ++j)
				blocks(i,j).fill(0);

		//хапольняю блоки
		for(int i = 0; i<size-2; ++i)
			for(int j=0; j< size-2; ++j)
			{
				blocks(i/psize,j/psize)[napr(i,j)]+=grad(i,j);
				//нормализация
				float norm = 0;
				for(int k=0; k< naps; ++k)
					norm+=pow(blocks(i/psize,j/psize)[k],2);
				norm=pow(norm,0.5);
				for(int k=0; k< naps; ++k)
					blocks(i/psize,j/psize)[k]/=(norm+0.001);
			}

		//пихаю в дескриптор
		for(int i = 0; i<n; ++i)
			for(int j=0; j< n; ++j)
				for(int k=0; k< naps; ++k)
				{
					one_image_features.push_back(blocks(i,j)[k]);
				}

		//------допольнительная чатсть!! Слава Императору! Смерть повстанцам!------
		//ЛБШ
		Matrix<float> lbs(h-2,w-2);
		for(int i = 1; i<h-1; ++i)
			for(int j=1; j< w-1; ++j)
			{
				int s=0;
				if(yark(i,j)<=yark(i-1,j))//upper
					s+=128;
				if(yark(i,j)<=yark(i-1,j+1))//upper right
					s+=64;
				if(yark(i,j)<=yark(i,j+1))//right
					s+=32;
				if(yark(i,j)<=yark(i+1,j+1))//right down
					s+=16;
				if(yark(i,j)<=yark(i+1,j))//down
					s+=8;
				if(yark(i,j)<=yark(i+1,j-1))//down left
					s+=4;
				if(yark(i,j)<=yark(i,j-1))//left
					s+=2;
				if(yark(i,j)<=yark(i-1,j-1))//left up
					s+=1;
				lbs(i-1,j-1)=s;
			}

		int kls=32; //размеры клеток
		int kln=(size-2)/kls; //количество клеток

		Matrix<array<float,256>> klets(kln,kln);
		for(int i = 0; i<kln; ++i)
			for(int j=0; j< kln; ++j)
				klets(i,j).fill(0);

		for(int i = 0; i<size-2; ++i)
			for(int j=0; j< size-2; ++j)
			{
				klets(i/kls,j/kls)[lbs(i,j)]++;
				//нормализация
				float norm = 0;
				for(int k=0; k< 256; ++k)
					norm+=pow(klets(i/kls,j/kls)[k],2);
				norm=pow(norm,0.5);
				for(int k=0; k< naps; ++k)
					klets(i/kls,j/kls)[k]/=(norm+0.001);
			}
		//пихаю в дескриптор
		for(int i = 0; i<kln; ++i)
			for(int j=0; j< kln; ++j)
				for(int k=0; k< 256; ++k)
				{
					one_image_features.push_back(klets(i,j)[k]);
				}

		//RAINBOW!!!
			//simg ,sh ,sw simg->GetPixel(w,h).Red

		int cn=8;//Число блоков
		//инициализация
		Matrix<array<float,4>> colors(cn,cn);
		for(int i = 0; i<cn; ++i)
			for(int j=0; j< cn; ++j)
				colors(i,j).fill(0);

		//запись цветов
		for(int i =0; i< sh; ++i)
			for(int j=0; j<sw; ++j)
			{
				colors(i*8/sh,j*8/sw)[0]+=simg->GetPixel(j,i).Red;
				colors(i*8/sh,j*8/sw)[1]+=simg->GetPixel(j,i).Blue;
				colors(i*8/sh,j*8/sw)[2]+=simg->GetPixel(j,i).Green;
				colors(i*8/sh,j*8/sw)[3]++;//количество записанных пикселей для будущего деления на это число
			}

		//усреднение
		for(int i = 0; i<cn; ++i)
			for(int j=0; j< cn; ++j)
			{
				colors(i,j)[0]/=colors(i,j)[3];
				colors(i,j)[1]/=colors(i,j)[3];
				colors(i,j)[2]/=colors(i,j)[3];
			}
		//пихаю в дескриптор
		for(int i = 0; i<cn; ++i)
			for(int j=0; j< cn; ++j)
				for(int k=0; k< 3; ++k)
				{
					one_image_features.push_back(colors(i,j)[k]/255);
				}


        features->push_back(make_pair(one_image_features, label));
        // End of sample code
    }
}

// Clear dataset structure
void ClearDataset(TDataSet* data_set) {
        // Delete all images from dataset
    for (size_t image_idx = 0; image_idx < data_set->size(); ++image_idx)
        delete (*data_set)[image_idx].first;
        // Clear dataset
    data_set->clear();
}

// Train SVM classifier using data from 'data_file' and save trained model
// to 'model_file'
void TrainClassifier(const string& data_file, const string& model_file) {
        // List of image file names and its labels
    TFileList file_list;
        // Structure of images and its labels
    TDataSet data_set;
        // Structure of features of images and its labels
    TFeatures features;
        // Model which would be trained
    TModel model;
        // Parameters of classifier
    TClassifierParams params;
    
        // Load list of image file names and its labels
    LoadFileList(data_file, &file_list);
        // Load images
    LoadImages(file_list, &data_set);
        // Extract features from images
    ExtractFeatures(data_set, &features);

        // PLACE YOUR CODE HERE
        // You can change parameters of classifier here
    params.C = 0.01;//0.01 original
    TClassifier classifier(params);
        // Train classifier
    classifier.Train(features, &model);
        // Save model to file
    model.Save(model_file);
        // Clear dataset structure
    ClearDataset(&data_set);
}

// Predict data from 'data_file' using model from 'model_file' and
// save predictions to 'prediction_file'
void PredictData(const string& data_file,
                 const string& model_file,
                 const string& prediction_file) {
        // List of image file names and its labels
    TFileList file_list;
        // Structure of images and its labels
    TDataSet data_set;
        // Structure of features of images and its labels
    TFeatures features;
        // List of image labels
    TLabels labels;

        // Load list of image file names and its labels
    LoadFileList(data_file, &file_list);
        // Load images
    LoadImages(file_list, &data_set);
        // Extract features from images
    ExtractFeatures(data_set, &features);

        // Classifier 
    TClassifier classifier = TClassifier(TClassifierParams());
        // Trained model
    TModel model;
        // Load model from file
    model.Load(model_file);
        // Predict images by its features using 'model' and store predictions
        // to 'labels'
    classifier.Predict(features, model, &labels);

        // Save predictions
    SavePredictions(file_list, labels, prediction_file);
        // Clear dataset structure
    ClearDataset(&data_set);
}

int main(int argc, char** argv) {
    // Command line options parser
    ArgvParser cmd;
        // Description of program
    cmd.setIntroductoryDescription("Machine graphics course, task 2. CMC MSU, 2014.");
        // Add help option
    cmd.setHelpOption("h", "help", "Print this help message");
        // Add other options
    cmd.defineOption("data_set", "File with dataset",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("model", "Path to file to save or load model",
        ArgvParser::OptionRequiresValue | ArgvParser::OptionRequired);
    cmd.defineOption("predicted_labels", "Path to file to save prediction results",
        ArgvParser::OptionRequiresValue);
    cmd.defineOption("train", "Train classifier");
    cmd.defineOption("predict", "Predict dataset");
        
        // Add options aliases
    cmd.defineOptionAlternative("data_set", "d");
    cmd.defineOptionAlternative("model", "m");
    cmd.defineOptionAlternative("predicted_labels", "l");
    cmd.defineOptionAlternative("train", "t");
    cmd.defineOptionAlternative("predict", "p");

        // Parse options
    int result = cmd.parse(argc, argv);

        // Check for errors or help option
    if (result) {
        cout << cmd.parseErrorDescription(result) << endl;
        return result;
    }

        // Get values 
    string data_file = cmd.optionValue("data_set");
    string model_file = cmd.optionValue("model");
    bool train = cmd.foundOption("train");
    bool predict = cmd.foundOption("predict");

        // If we need to train classifier
    if (train)
        TrainClassifier(data_file, model_file);
        // If we need to predict data
    if (predict) {
            // You must declare file to save images
        if (!cmd.foundOption("predicted_labels")) {
            cerr << "Error! Option --predicted_labels not found!" << endl;
            return 1;
        }
            // File to save predictions
        string prediction_file = cmd.optionValue("predicted_labels");
            // Predict data
        PredictData(data_file, model_file, prediction_file);
    }
}
