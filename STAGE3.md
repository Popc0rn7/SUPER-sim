# Stage 3 Humble / PX4 仿真适配

feat/sim 服务 uav-lesson 的 SUPER＋PX4/GNSS 走廊流程，仅构建 super_planner、rog_map、mars_uav_sim/mars_quadrotor_msgs。

必要扩展：ROS 2 构建/编译兼容、仿真时钟与绝对配置路径、最终目标静止保持、初次规划失败限频重试、引导路径分段、中心高度边界和 /tmp/stage3-super 日志路径。

高度边界由 SUPER_CENTER_Z_MIN/MAX 指定，未设置时不限制中心高度。启动前创建 /tmp/stage3-super。课程配置、雷达/PX4 适配、场景、容器脚本和验收留在 uav-lesson/stage3/super/。

课程输入：/cloud_registered 世界坐标点云、/lidar_slam/odom 里程计（由 PX4/GNSS 转换提供）、/goal_pose 目标。输出：/planning/pos_cmd。世界坐标为 ENU，单位为米、秒；详细参数由课程 YAML 提供。

终点保持测试：

```sh
g++ -std=c++17 -I super_planner/include super_planner/test/terminal_hold_test.cpp -o /tmp/super-terminal-test
/tmp/super-terminal-test
```

构建和飞行步骤见课程 docs/stage3/Hands-on.md；编译成功不等于飞行验收。
