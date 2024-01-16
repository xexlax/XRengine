# XRENGINE
支持可视化编程的简易游戏引擎

## 功能简介
1. 渲染系统（OpenGL/Vulkan）
   支持pbr、阴影映射、延迟渲染、后处理（SSAO、SSRT）

2. Entity-Component-System
   9种内置Component以及蓝图Component

3. 刚体物理系统（JoltPhysics）
   支持简单几何体的碰撞模拟、碰撞检测和射线检测

4. 三维音效系统（OpenAL）

5. 可视化节点脚本-蓝图系统
   70+节点，支持序列化

6. GUI编辑器
   常见编辑功能、Undo/Redo指令栈

7. OpenXR基础框架


## 项目结构
1. \XREngine 储存核心源码，包括桌面游戏运行的全部功能支持，编译为lib
2. \XRE-Editor 引擎编辑器，默认的启动项目
3. \XRE-Launcher 桌面项目的启动器
4. \XRE-OpenXR XR项目的启动器，需要连接VR设备后起用，不完善
5. \XRE-Sandbox 目前用于测试Vulkan多线程渲染，不完善


5. \Assets 引擎通用资产的存放路径
6. \Projects 用户项目及资产的存放路径


## 构建方法
1. 克隆源码和gitplugin中的第三方库
2. 将backup中内容（含CMakelist）覆盖\XREngine\3rdparty
3. 使用CMake构建