#include<iostream>
#include<ctime>

using namespace std;

#include<Eigen/Core>
#include<Eigen/Dense>
//the operator inverse, eigenvalue of the matrix

#define MATRIX_SIZE 50

int main(int argc, char** argv) {
    // Eigen is a template class, and you can take it as a basic data format.
    // the arguments are: data type, rows, cols
    Eigen::Matrix<float,2,3> matrix_23;
    matrix_23<<1, 2, 3, 4, 5, 6;
    
    Eigen::Vector3d vector_3;
    // In esssece, this is same sa: Eigen::Matrix<double, 3, 1>
    
    Eigen::Matrix3d matrix_33 = Eigen::Matrix3d::Zero(); 
    // Matrix3d is equal as the Matrix<double, 3, 3>
    // Init as zero matrix
    
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> matrix_dynamic;
    // If we not know the matrix size exactly, you could set it to be Dynamic
    
    cout<<matrix_23<<endl;
    
    for(int i=0; i< 2; i++)
        for(int j = 0; j < 3; j++)
            cout<< matrix_23(i, j)<<" ";
    
    vector_3 << 1, 2, 3;
    Eigen::Matrix<double, 3, 1> result = matrix_33 * vector_3;
    cout<<result<<endl;
    // Rember that the EIgen could not handle the operation between different date type
    // you should explict define or change it suitable
    Eigen::Matrix<double, 2, 1> result_correct = matrix_23.cast<double>() * vector_3;
    cout<< result_correct<<endl;
    
    matrix_33 = Eigen::Matrix3d::Random();
    // solve the eigen value
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigen_solver (matrix_33.transpose() * matrix_33);
    cout<<"Eigen vectors"<< eigen_solver.eigenvectors()<<endl;
    
    // Here, we will show how to solve the equaptions
    // matrix_NN *x = v_Nd
    // Dirctly solve the inverse is accaptable, but it is quite time-comsuming
    Eigen::Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NN = Eigen:: MatrixXd::Random(MATRIX_SIZE,MATRIX_SIZE);
    Eigen::Matrix<double, MATRIX_SIZE, 1> vector_Nd = Eigen::MatrixXd::Random(MATRIX_SIZE,1);
    
    clock_t time_stt = clock();
    Eigen::Matrix<double, MATRIX_SIZE, 1> x = matrix_NN. inverse() * vector_Nd;
    cout<< "time used in the normal inverse is "<< (clock()-time_stt)<<endl;
    
    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(vector_Nd);
     cout<< "time used in the QR Compsition is "<< (clock()-time_stt)<<endl;
    
    
    return 0;
    
}
