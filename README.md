2d流体引擎
titmouse是一个开源的2d流体引擎（个人玩具），项目启动于2021年10月3日，致力于实现所有主流流体模拟算法

- C++ 11 （使用C++ 11标准）
- basic vector operator（基本的向量操作）
- basic array struct（基本的数组结构）
- basic dense matrix struct and operator（基本的稠密矩阵结构及操作）
- CSR sparse matrix struct and operator（CSR稀疏矩阵结构以及相关操作）
- Iterative Solve Linea System：
  -  Steepest Descent method(最速下降法)
  - ConjugateGradient method(共轭梯度法)
  - Jaocobi method(雅可比迭代法)
  - Gauss-Seidel method(高斯赛德法)
- neighborSearch struct in O(kn)（基于网格的粒子邻近搜索结构）
- collision detection（2D情况下任意几何形状的碰撞检测与处理）
- FDM Grids such as CenteredGrid (有限差分计算用到的均匀网格结构)
- PIC/FLIP scheme (PIC/FLIP映射方案)
- MarchingSquare algorthm 
- Framework or Api(使用的框架或接口): 
  - OpenGL/GLUT
  - OpenMP
 
