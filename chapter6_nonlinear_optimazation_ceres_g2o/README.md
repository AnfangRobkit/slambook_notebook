这一讲介绍了用G2O库，具体的安装操作请参看高翔的SLAM14讲，本文只针对书中未提及的一些bug进行一些总结。

## g2o CMAKE编译失败

在使用CMAKE编译时会出现一下报错:

```cmake
By not providing "FindG2O.cmake" in CMAKE_MODULE_PATH this project has asked CMake to find a package configuration file provided by "G2O", but CMake did not find one.

Could not find a package configuration file provided by "G2O" with any of the following names:
	G2OConfig.cmake
	g2o-config.cmake
 Add the installation prefix of "G2O" to CMAKE_PREFIX_PATH or set "G2O_DIR"to a directory containing one of the above files.  If "G2O" provides separate development package or SDK, be sure it has been installed.
```

原因在于*CMakeLists.txt*中，虽然定义了CMAKE_MODULE_PATH，但没有对应的文件和文件夹，也就是没有找到FindG2O.cmake文件

解决办法：找到FINDG2O.CMAKE文件的位置，一般在g2o安装包中的"cmake_modules"文件夹中，并把整个文件夹复制到相应工程的下面，与build文件夹在同一目录，然后编译可以正常通过。

## g2o 版本升级导致的问题

```cpp
error: no matching function for call to ‘g2o::BlockSolver<g2o::BlockSolverTraits<3, 1> >::BlockSolver(g2o::BlockSolver<g2o::BlockSolverTraits<3, 1> >::LinearSolverType*&)’
     Block* solver_ptr = new Block(linearSolver);
                                               ^

error: no matching function for call to ‘g2o::OptimizationAlgorithmLevenberg::OptimizationAlgorithmLevenberg(Block*&)’
     g2o::OptimizationAlgorithmLevenberg* solver=new g2o::OptimizationAlgorithmLevenberg(solver_ptr);
```

查阅g2o源码的构造函数发现:

```cpp
BlockSolver<Traits>::BlockSolver(std::unique_ptr<LinearSolverType> linearSolver)
    :   BlockSolverBase(),
        _linearSolver(std::move(linearSolver))
```

注意到新版本的g2o里使用了**std::unique_ptr** 

故而修改代码如下:

```cpp
std::unique_ptr<Block::LinearSolverType> linearSolver(new g2o::LinearSolverDense<Block::PoseMatrixType>());
std::unique_ptr<Block> solver_ptr ( new Block ( std::move(linearSolver))); g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg(std::move(solver_ptr));
```

解决问题，完美编译。

本人所有修改版本代码均已上床GitHub,欢迎一起探讨学习:

https://github.com/AnfangRobkit/slambook_notebook



## References

[1]https://www.cnblogs.com/xueyuanaichiyu/p/7921382.html

[2]https://blog.csdn.net/weixin_36673043/article/details/85345886