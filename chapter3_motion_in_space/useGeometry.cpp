#include<iostream>
#include<cmath>

using namespace std;

#include<Eigen/Core>
#include<Eigen/Geometry>
//Eigen/Geometry offers us the option for rotation and translation

int main(int argc, char** argv) {
    Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();
    
    // use AngleAxis to represent the rotation vector
    Eigen::AngleAxisd rotation_vector(M_PI/4, Eigen::Vector3d(0,0,1));
    // alongside the z-axis to rotate about 45 degree
    
    cout.precision(3);
    cout<<"rotation matrix= \n"<<rotation_vector.matrix()<<endl;

    rotation_matrix = rotation_vector.toRotationMatrix();
    cout<<"after toRotationMatrix\n"<<rotation_matrix<<endl;
    rotation_matrix = rotation_vector.matrix();
    cout<<"after .matrix()\n"<<rotation_matrix<<endl;
    
    Eigen::Vector3d v(1,0,0);
    Eigen::Vector3d v_rotated = rotation_matrix * v;
    cout<<"(1,0,0) after rotation\n" << v_rotated.matrix().transpose()<<endl;
    
    Eigen::Vector3d euler_angle = rotation_matrix.eulerAngles(2,1,0);
    //Here we should speific which order of the euler_angle should be used
    //Here: yaw pitch roll
    cout<<"yaw pitch roll\n"<<euler_angle.transpose()<<endl;
    
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity(); //Isometry3d will result into a 4*4 matrix
    T.rotate(rotation_vector); //rotate based on the rotation_vector
    T.pretranslate(Eigen::Vector3d(1,3,4));
    cout<<"Transform Matrx in Eulersche Form\n"<<T.matrix()<<endl;
    
    Eigen::Vector3d v_transformed = T * v;
    cout<<"v transformed\n" <<v_transformed<<endl;
    
    Eigen::Quaterniond q = Eigen::Quaterniond(rotation_vector);
    cout<<"Quaternion = \n"<<q.coeffs()<<endl;
    // Note that the coeffs() order is (x,y,z,w), w is reale part
    
    q = Eigen::Quaterniond(rotation_matrix);
    cout<<"Quaternion = \n"<<q.coeffs()<<endl;
    
    v_rotated = q * v;
    cout<<"(1,0,0) after rotation with Quaternion\n" << v_rotated.matrix().transpose()<<endl;
    
    return 0;
    
    
}
