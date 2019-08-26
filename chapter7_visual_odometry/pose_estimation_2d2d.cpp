#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

void find_feature_matches( const Mat& img_1, const Mat& img_2, std::vector<KeyPoint>& keypoints_1,
std::vector<KeyPoint>& keypoints_2, std::vector<DMatch>& matches);

void pose_estimation_2d2d(std::vector<KeyPoint>& keypoints_1,
std::vector<KeyPoint>& keypoints_2, std::vector<DMatch>matches, Mat& R, Mat& t );

Point2d pixel2cam (const Point2d& p, const Mat& K);

int main( int argc, char** argv)
{
    if(argc !=3)
    {
        cout<<"usage: pose_estimation_2d2d img1 img2"<<endl;
        return 1;
    }
    Mat img_1 = imread(argv[1], cv::IMREAD_COLOR);
    Mat img_2 = imread(argv[2], cv::IMREAD_COLOR);

    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;

    find_feature_matches(img_1, img_2, keypoints_1, keypoints_2, matches);
    cout<<" The number of "<<matches.size()<<" matches have been found"<<endl;

    // Estimate the motion between two images
    Mat R,t;
    pose_estimation_2d2d(keypoints_1, keypoints_2, matches, R, t);

    // Check the E= t^ R* scale
    Mat t_x = (Mat_<double>(3,3) << 
    0, -t.at<double>(2,0), t.at<double>(1,0),
    t.at<double>(2,0), 0, -t.at<double>(0,0),
    -t.at<double>(1,0), t.at<double>(0,0), 0);
    cout<<"t^R= "<<endl<<t_x *R << endl;


    // Check the constrains of the epipolar constrains
    Mat K = (Mat_<double>(3,3)<<520.0, 325.1,  0, 521.0, 249.7, 0,0,1);
    for(DMatch m:matches)
    {
        Point2d pt1 = pixel2cam(keypoints_1[m.queryIdx].pt, K);
        Mat y1 = (Mat_<double>(3,1)<<pt1.x, pt1.y,1);
        Point2d pt2 = pixel2cam(keypoints_2[m.trainIdx].pt, K);
        Mat y2 = (Mat_<double>(3,1)<<pt2.x, pt2.y, 1);
        Mat d = y2.t() * t_x * R*y1;
        cout<<" epipolar constraint = "<< d<< endl;
    }
    return 0;
}
void find_feature_matches( const Mat& img_1, const Mat& img_2, std::vector<KeyPoint>& keypoints_1,
std::vector<KeyPoint>& keypoints_2, std::vector<DMatch>& matches)
{
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = cv::ORB::create();
    Ptr<DescriptorExtractor>descriptors = cv::ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    // First steo: Oriented Fast Corner Detection
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    // Based on the corner detection results to set BRIEF descriptor
    descriptors->compute(img_1, keypoints_1, descriptors_1);
    descriptors->compute(img_2, keypoints_2, descriptors_2);

    // Thirdly, assign the descriptor between the two images, using Hamming distance
    vector<DMatch> match;
    matcher->match(descriptors_1, descriptors_2, match);

    // 
    double min_dist = 10000, max_dist =0;
    // find the minimal and the maximal distance
    for(int i = 0; i< descriptors_1.rows; i++)
    {
        double dist = match[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }

    cout<<"Max distance :"<<max_dist<<endl<<"Min distance :"<<min_dist<<endl;

    // when the distance between descriptors is bigger than 2 times of min_dist, we could assume no relations
    // sometimes the distance could be so smaller, we talk the 30 as our lower bound
    for( int i=0; i< descriptors_1.rows;i++)
    {
        if(match[i].distance<=max(2*min_dist, 30.0))
        {
            matches.push_back(match[i]);
            //cout<<"find +1"<<endl;
        }
    }
    
}

Point2d pixel2cam(const Point2d &p, const Mat &K) 
{
    return Point2d(
        (p.x - K.at<double>(0,2)/K.at<double>(0,0))/K.at<double>(0,0),
        (p.y - K.at<double>(1,2)/K.at<double>(0,0))/K.at<double>(1,1)
    );
}

void pose_estimation_2d2d(std::vector<KeyPoint>& keypoints_1,
std::vector<KeyPoint>& keypoints_2, std::vector<DMatch>matches, Mat& R, Mat& t )
{
    // Instrinic Parameter of the camera
    // TUM Freiburg 2
    Mat K = (Mat_<double>(3,3)<<520.9,0,325.1,0,521.0,259.7,0,0,1);

    // Convert the corresponding points into vector<Point2f>
    vector<Point2f> points1;
    vector<Point2f> points2;

    // cout<<matches.size()<<endl;

    for(int i=0; i<(int) matches.size();i++)
    {
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        points2.push_back(keypoints_2[matches[i].trainIdx].pt);
    }

    // cout<<points1<<"  "<<points2<<endl;

    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat(points1,points2,FM_8POINT);
    cout<<"fundamental matrix is "<<endl<<fundamental_matrix<<endl;

    Point2d principal_point(325.1, 249.7); 
    double focal_length = 521;      
    Mat essential_matrix;
     essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);
    cout << "essential_matrix is " << endl << essential_matrix << endl;
    
    recoverPose ( essential_matrix, points1, points2, R, t, focal_length, principal_point );
    cout<<"R is "<<endl<<R<<endl;
    cout<<"t is "<<endl<<t<<endl;

}