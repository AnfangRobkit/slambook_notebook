#include<iostream>
#include<cmath>
#include<Eigen/Core>
#include<Eigen/Geometry>
using namespace std;

#include "sophus/so3.h"
#include "sophus/se3.h"

int main(int argc, char** argv){
    Eigen::Matrix3d R = Eigen::AngleAxisd(M_PI/2, Eigen::Vector3d(0,0,1)).toRotationMatrix();
    Sophus::SO3 SO3_R(R); //SO(3) could be constructed directly from the rotation Matrix3d
    Sophus::SO3 SO3_v(0,0, M_PI/2); //or from the rotate Vector
    Eigen::Quaterniond q_(R);
    Sophus::SO3 SO3_Q(q_); //or from the Quaternond
    
    cout<<"SO(3) from the matrix\n"<<SO3_R<<endl;
    cout<<"SO(3) from the vetcor\n"<<SO3_v<<endl;
    cout<<"SO(3) from the Quaternond\n"<<SO3_Q<<endl;
    //Note: 从输出的形式可以看出，虽然SO3是李群，是旋转矩阵，但是输出形式还是向量（被转化成李代数输出）
    
    
    // Here we use .log to chenge the Lie Group to Lie algebra
    Eigen::Vector3d so3 = SO3_R.log();
    cout<<"so3 = \n" <<so3.transpose()<<endl;
    
    // Use hat to get the skew-symmetric matrix
    cout<<"so3 hat =\n"<<Sophus::SO3::hat(so3)<<endl;
    
    cout<<"so3 hat vee\n" << Sophus::SO3::vee(Sophus::SO3::hat(so3)).transpose()<<endl;
    
    Eigen::Vector3d update_so3(1e-4,0,0);
    Sophus::SO3 SO3_update = Sophus::SO3::exp(update_so3) * SO3_R;
    cout<<"SO3 update\n"<<SO3_update<<endl;
    
    
    
    cout<<"****************"<<endl;
    // blow are the operations on SE3
    Eigen::Vector3d t(1,0,0);
    Sophus::SE3 SE3_Rt(R, t);
    Sophus::SE3 SE3_Qt(q_, t);
    cout<<"SE3 from R,t\n"<<SE3_Rt.matrix()<<endl;
    cout<<"SE3 from Q,t\n"<<SE3_Qt<<endl;
    
    typedef Eigen::Matrix<double, 6, 1>Vector6d;
    Vector6d se3 = SE3_Rt.log();
    cout<<"se3= \n"<<se3.transpose()<<endl;
    // In the Sophus se3 is a 6-dim Vector, and the translation is ahead of rotation
    cout<<"se3 hat= \n"<<Sophus::SE3::hat(se3)<<endl;
    
    Vector6d update_se3;
    update_se3.setZero();
    update_se3(0,0)=1e-4d;
    Sophus::SE3 SE3_updated = Sophus::SE3::exp(update_se3) * SE3_Rt;
    cout<<"SE3 update= \n"<<SE3_updated.matrix()<<endl;
    
    
    return 0;

    
}
