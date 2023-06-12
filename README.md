# XRENGINE
支持可视化编程和OpenXR的简易游戏引擎

## 功能简介
1. 三维场景渲染系统
2. ECS
3. 刚体物理系统
4. 三维音效系统
5. 可视化节点脚本-蓝图系统
6. GUI编辑器
7. OpenXR基础框架


## 项目结构
1. \XREngine 储存核心源码，包括桌面游戏运行的全部功能支持，编译为lib
2. \XRE-Editor 引擎编辑器，默认的启动项目
3. \XRE-Launcher 桌面项目的启动器
4. \XRE-OpenXR XR项目的启动器，需要连接VR设备后起用


5. \Assets 引擎通用资产的存放路径
6. \Projects 用户项目及资产的存放路径


## 构建方法
1. 克隆源码和gitplugin中的第三方库
2. 将backup中内容（含CMakelist）覆盖\XREngine\3rdparty
3. 使用CMake构建