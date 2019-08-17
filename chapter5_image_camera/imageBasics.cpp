#include<iostream>
#include<chrono>

using namespace std;

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

int main(int argc, char** argv) {
    cv::Mat image;
    image = cv::imread(argv[1]);
    if(image.data == nullptr) {
        cout<<"the file "<<argv[1]<<" does not exit" <<endl;
        return 0;
    }
    cout<<"the image width= "<<image.cols<<" the image length= "<<image.rows<<" the channels= "<<image.channels()<<endl;
    cv::imshow("image",image);
    cv::waitKey(0);
    
    // The std::chrono will be used to time the algorithms
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    for(size_t y=0; y<image.rows;y++) {
        for(size_t x=0;x<image.cols; x++) {
            unsigned char* row_ptr = image.ptr<unsigned char>(y);
            //row_ptr is the pointer of the no.y row
            unsigned char* data_ptr = &row_ptr[x*image.channels()];
            // data_ptr points to the pixel
            for(int c=0;c!=image.channels();c++){
                unsigned char data = data_ptr[c];
                //data is the value of channel c of the I(x,y)
            }
        }
    }
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2-t1);
    cout<<"time_cosumd"<<time_used.count()<<" s"<<endl;
    
    //shallow copy
    cv::Mat image_another = image;
    image_another(cv::Rect(0,0,100,100)).setTo(0);
    cv::imshow("image",image);
    cv::waitKey(0);
    
    //deep copy with clone method
    cv::Mat image_clone = image.clone();
    image_clone(cv::Rect(0,0,100,100)).setTo(255);
    cv::imshow("image",image);
    cv::imshow("image_clone", image_clone);
    cv::waitKey(0);
    
    cv::destroyAllWindows();
    return 0;
    
}
