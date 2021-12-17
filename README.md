2d流体引擎
titmouse是一个开源的2d流体引擎（个人玩具），项目启动于2021年10月3日。

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
- Implicit/Explicit transform (显式/隐式曲面转换)
- SDFs(常用几何形状的sdf计算)
- FDM Grids such as CenteredGrid (有限差分计算用到的均匀网格结构)
- PIC/FLIP scheme (PIC/FLIP映射方案)
- Semi-Lagrangian advection solver(半拉格朗日对流求解器)
- Pressure Possion Equation Solve (压力泊松方程求解)
- MarchingSquare algorthm 
- Framework or Api(使用的框架或接口): 
  - OpenGL/GLUT
  - OpenMP
 
实现的流体算法：
- FLIP/PIC + 压力泊松方程求解自由表面流
- Euleian LevelSet free surface flow （纯欧拉视角下的levelset自由表面流体模拟）
- SPH algorthm （光滑粒子动力学算法）（SPH的参数我也是醉了）


- 1.FLIP算法
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/titmouse2d_firstdemo.gif)

- 2.advection测试
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/titmouse_advection_without_.gif)

- 3.level set free surface flow
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/levelset2.gif)

- 4.Marching cube2d
- ![Image]( https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/marching%20cube2d.gif)

- 5.SSPH
- ![Image](  https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/ssph2d.gif)

 
