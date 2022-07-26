titmouse是一个2d离线物理引擎（个人玩具），由于程序性能受限，所有算例都是在低分辨率下解算。titmouse2d的目标是实现各种模拟算法的2d版本。

- C++ 11
- 基本的向量操作
- 基本的数组结构
- 基本的稠密矩阵结构
- CSR稀疏矩阵结构
- Iterative Solve Linea System：
  - Steepest Descent method
  - ConjugateGradient method
  - Jaocobi method
  - Gauss-Seidel method
  - Relaxation jacobi
  - Successive over relaxation
  - Jacobi PCG
  - ICCG
- 基于网格的粒子邻近搜索结构
- 2D情况下任意几何形状的碰撞检测与处理
- 显式/隐式曲面转换
- 常用几何形状的sdf计算
- 有限差分计算用到的均匀网格结构
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
- Shallow Wave Solver

实现的弹性体/刚体算法：
- Constraint Dynamics （Mass-spring system）
- 单个刚体动力学
效果图
- 1.FLIP算法（没有在网格上对边界条件进行处理，因此边界处速度有散度）
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/titmouse2d_firstdemo.gif" width="300">

- 2.advection测试
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/titmouse_advection_without_.gif" width="300">
- 3.level set free surface flow
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/levelset2.gif" width="300">

- 4.Marching cube2d
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/marching%20cube2d.gif" width="300">

- 5.SSPH
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/ssph2d.gif" width="300">

- 6.PIC
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/pic.gif" width="300">

- 7.Vortex Particles Method
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/vortexparticles_titmouse.gif" width="300">

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
- 14.kama vortex
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/kamavortex.gif)
- 15.shallow wave solver
- ![Image](https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/swe_small.gif)
- 16.Constrained Dynamics（这玩意弹簧系数调大了是可以做刚体的，为什么会碎呢，是因为我的约束条件的问题，超过一定距离就把约束撤掉了）
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/constrained_dynamics.gif" width="300">
- 17.RigidBody(单个刚体的模拟，多个刚体之间碰撞尚未实现)
- <img src="https://github.com/casenoone/titmouse2d/blob/master/titmouse2d/src/gif/rigidbody1.gif" width="300">
即将实现的算法或数据结构：
- PCISPH
- BVH加速结构
- Clebsch gauge fluid solver
- Multigrid Method
- FVM（有限体积法）
- TreeCode多体问题加速
- FMM(快速多极展开)
- 更适合物理模拟的稀疏矩阵结构



 
