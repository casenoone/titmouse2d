2d流体引擎
titmouse是一个2d离线流体引擎（个人玩具）

- C++ 11
- 基本的向量操作
- 基本的数组结构
- 基本的稠密矩阵结构及操作
- CSR稀疏矩阵结构以及相关操作
- Iterative Solve Linea System：
  - Steepest Descent method(最速下降法)
  - ConjugateGradient method(共轭梯度法)
  - Jaocobi method(雅可比迭代法)
  - Gauss-Seidel method(高斯赛德法)
  - Relaxation jacobi
  - Successive over relaxation
- 基于网格的粒子邻近搜索结构
- 2D情况下任意几何形状的碰撞检测与处理
- 显式/隐式曲面转换
- 常用几何形状的sdf计算
- 有限差分计算用到的均匀网格结构
- PIC/FLIP/APIC格式
- 半拉格朗日对流求解器
- 压力泊松方程求解
- 涡粒子法
- LBM Method(格子-玻尔兹曼方法)
- MarchingSquare algorthm 
- Framework or Api(使用的框架或接口): 
  - OpenGL/GLUT/GLAD/GLFW
  - OpenMP
 
实现的流体算法：
- APIC/FLIP/PIC + 压力泊松方程求解自由表面流
- Euleian LevelSet free surface flow （纯欧拉视角下的levelset自由表面流体模拟）
- SPH algorthm （光滑粒子动力学算法）（SPH的参数我也是醉了,凑合着玩玩吧）
- Vortex Method(涡粒子法模拟流体)
- LBM（格子-玻尔兹曼方法）
- MPM
- PBF
- IISPH(隐式不可压缩SPH)
- Eulerian Smoke 


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

- 6.PIC
- ![Image](  https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/pic.gif)

- 7.Vortex Particles Method
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/vortexparticles_titmouse.gif)

- 8.LBM(湍流模型模拟顶盖流动)
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/LBM_turbulence_with_circle.gif)



- 9.PBF(参数和SPH一样恶心。。。)
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/pbf2.gif)

- 10.IISPH
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/iisph2.gif)

- 11.APIC
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/apic.gif)

- 12.MLS-MPM(用了双线性插值，粘成这个样子，中间还有一块蜜汁突起，也可能是代码写错了)
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/mls_mpm_linear.gif)

- 13.Eulerian Smoke(双线性插值，midpoint，边界还有点bug没解决，还没有加vorticity force)
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/eulerian_smoke.gif)

- 14.shallow wave solver
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/swe_small.gif)

即将实现的算法或数据结构：
- PCISPH
- BVH加速结构
- Explicit/Implicit Mass Spring System
- Clebsch gauge fluid solver
- Multigrid Method
- FVM（有限体积法）
- FMM(快速多极展开)
- 更适合物理模拟的稀疏矩阵结构



 
