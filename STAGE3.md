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

A* 远目标裁剪：成功裁剪到局部窗口内的目标仅表示局部进展，搜到后返回
`REACH_HORIZON`；未裁剪的真实目标仍返回 `REACH_GOAL`。前者不允许
`PathSearch` 直接追加未经搜索的远端目标。`planning_horizon` 的含义不变。
在 Humble 构建环境向 colcon 添加 `--cmake-args -DSUPER_BUILD_PATH_SEARCH_TEST=ON`
可生成 `build/super_planner/astar_clipped_goal_test`。构建后在 Docker 的 `/super_ws` 执行
`ctest --test-dir build/super_planner -R astar_clipped_goal --output-on-failure`。
测试脚本从上游配置生成临时无障碍地图，使用独立 DDS 域与日志，不依赖 AirSim 或
历史飞行产物；覆盖近目标、裁剪目标、距离截止及连续调用，不替代障碍场景飞行验收。
独立运行也可用 `python3 src/super_planner/test/run_astar_clipped_goal.py
build/super_planner/astar_clipped_goal_test`（同一行命令）。
