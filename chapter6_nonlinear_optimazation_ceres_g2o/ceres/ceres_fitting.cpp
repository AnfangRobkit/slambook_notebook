#include<iostream>
#include<opencv2/core/core.hpp>
#include<ceres/ceres.h>
#include<chrono>

using namespace std;
// Define the cost Function model:
// we define a class and inside one operator() with template to make the class be one Functor.
struct CURVE_FITTING_COST
{
    CURVE_FITTING_COST(double x, double y) : _x(x), _y(y) {}
    // calculation of the residual
    template <typename T>
    bool operator() (const T* const abc, T*  residual) const {
        //abc stands for the parameter of model, with three diemension
        // operates for y-exp(ax^2 + bx +c)
        residual[0] = T(_y) - ceres::exp(abc[0] * T(_x) * T(_x) + abc[1] * T(_x) +abc[2]);
        return true;
    }
    const double _x, _y; 
};
int main(int argc, char** argv)
{
    double a=1.0, b=2.0, c=1.0; // the ground truth
    int N=100; //the data number
    double w_sigma =  1.0; // the sigma of the noise
    cv::RNG rng;    // random generator in OpenCV
    double abc[3] = {0,0,0};

    vector<double> x_data, y_data;
    cout<<"generating data"<<endl;
    for(int i=0; i<N; i++) 
    {
        double x=i/100.0;
        x_data.push_back(x);
        y_data.push_back(exp(a*x*x + b*x +c)+ rng.gaussian(w_sigma));
        cout<<x_data[i]<<" "<<y_data[i]<<endl;
    }
    // contrust the least square problem
    ceres::Problem problem;
    for(int i=0; i<N; i++) 
    {   
        // add residual to the problem
        problem.AddResidualBlock(new ceres::AutoDiffCostFunction<CURVE_FITTING_COST,1,3> (
            new CURVE_FITTING_COST( x_data[i], y_data[i])),//use auto diff: type of error, out dim, input dim
            nullptr,// kernel function, here set to be null
            abc );
    }
    // config the solver
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;// set the output to stdout

    ceres::Solver::Summary sumary;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    ceres::Solve(options, &problem, &sumary);//begin optimize
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double>time_used = chrono::duration_cast<chrono::duration<double>>(t2-t1);
    cout<<"solve time cost= "<<time_used.count()<<" seconds"<<endl;

    cout<<sumary.BriefReport()<<endl;
    cout<<"estimated a, b,c ="<<endl;
    for(auto a: abc) cout<<a<<" ";
    return 0;
}
