# 第三方组件与开源许可证清单（SBOM 初版 - M0）

| 组件名称 | 版本 / 提交 | 许可证 | 商业闭源兼容义务 | 核查状态 |
| :--- | :--- | :--- | :--- | :--- |
| PX4-Autopilot | `v1.17.0` (`d6f12ad1c4f7`) | BSD-3-Clause | 允许闭源；发行时须保留版权声明、许可文本及免责声明 | 已核查公开许可 |
| Apache NuttX | PX4 子模块 `fb2fadf6f599` | Apache-2.0 | 允许闭源；修改文件须保留变更说明，打包分发须附带 Apache-2.0 文本及 NOTICE | 已核查公开许可 |
| GD32-MCU-IOT/gd32_nuttx | `master` (`a71c8d5`) | Apache-2.0 | 仅作参考/最小审计补丁来源；须逐文件核查是否混入非 Apache-2.0 厂商头文件 | 待离线源码逐文件审计 |
| GigaDevice GD32H7xx Firmware Library | 待下载官方版本 | GigaDevice 厂商许可（通常为 BSD 类或器件绑定许可） | **不可默认假定无条件兼容**；须在获取官方 SDK 后核对 `LICENSE` 与芯片绑定条款 | **待核查** |
| MAVLink C Headers (v2) | PX4 子模块 | MIT | 允许闭源；保留 MIT 版权与许可声明 | 已核查 |
