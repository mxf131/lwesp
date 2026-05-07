# 项目开发规范 — LwESP

> ⚠️ 本文件定义当前项目的专属规范，优先级高于全局规范。
> ⚠️ 禁止将其他项目的习惯带入本项目。

## 项目边界声明

- **项目名**：LwESP (Lightweight ESP-AT parser library)
- **路径**：`d:\study\lwesp-develop`
- **技术栈**：C99 | CMake ≥ 3.22 | Ninja 构建 | clang-format 代码格式化
- **目标平台**：嵌入式（ARM Cortex-M, AVR, PIC）+ Win32/Linux/macOS 开发环境
- **支持设备**：ESP32, ESP32-C2, ESP32-C3, ESP32-C6, ESP8266
- **许可证**：MIT
- **数据库**：无
- 当前对话的所有代码修改必须限定在本项目范围内

## 模块结构

| 模块 | 路径 | 职责 |
|------|------|------|
| **core** | `lwesp/src/lwesp/` | 核心库：初始化、连接管理、AT 解析、事件、内存、缓冲区等（30 个 .c 文件） |
| **api** | `lwesp/src/api/` | 高级顺序 API（netconn 模块） |
| **apps** | `lwesp/src/apps/` | 应用层：HTTP Server、MQTT Client |
| **cli** | `lwesp/src/cli/` | 命令行接口 |
| **system** | `lwesp/src/system/` | 平台移植层（Win32/POSIX/CMSIS-OS/FreeRTOS/ThreadX/STM32 系列） |
| **include** | `lwesp/src/include/` | 所有公共头文件 |
| **dev** | `dev/` | 开发/测试入口（main.c, lwesp_opts.h, VS 工程） |
| **examples** | `examples/` | 示例代码（Win32/STM32/POSIX 平台） |
| **snippets** | `snippets/` | 可复用代码片段 |
| **docs** | `docs/` | ReadTheDocs Sphinx 文档源 |

## 项目特有命名约定

- **类型/结构体**：小写下划线 + `_t` 后缀，如 `lwesp_conn_t`, `lwesp_mode_t`
- **枚举值**：驼峰式 `lwesp` 前缀，如 `lwespOK`, `lwespERR`；或全大写 `LWESP_` 前缀，如 `LWESP_DEVICE_ESP32`
- **公共函数**：`lwesp_` 前缀，如 `lwesp_init()`, `lwesp_conn_send()`
- **内部函数**：`lwespi_` 前缀或 `prv_` 前缀
- **宏/常量**：全大写 `LWESP_` 前缀，如 `LWESP_CFG_OS`, `LWESP_DBG_LVL_SEVERE`
- **头文件守卫**：`LWESP_HDR_*_H` 或 `LWESP_*_HDR_H`
- **文件名**：`lwesp_*.c` / `lwesp_*.h`

## 日志模块名约定

- 使用 `LWESP_DEBUGF()` 宏
- 格式：`"[LWESP MODULE] message\r\n"`
- 模块标识宏：`LWESP_CFG_DBG_INIT`, `LWESP_CFG_DBG_CONN`, `LWESP_CFG_DBG_NETCONN`, `LWESP_CFG_DBG_MQTT` 等
- 级别组合：`LWESP_DBG_LVL_WARNING | LWESP_DBG_TYPE_TRACE`

## 项目特殊规则

- **构建配置通过 `lwesp_opts.h` 覆写**：默认值在 `lwesp_opt.h`，用户覆写在 `lwesp_opts.h`
- **条件编译模式**：`#if LWESP_CFG_FEATURE || __DOXYGEN__` ... `#endif /* LWESP_CFG_FEATURE */`
- **遵循上游代码风格**：[MaJerle/c-code-style](https://github.com/MaJerle/c-code-style)
- **返回类型单独一行**（函数定义），大括号 Attach 风格
- **禁止修改 `examples/lib/` 下的第三方库**（FreeRTOS、ST HAL）
- **内存管理统一接口**：`lwesp_mem_malloc` / `lwesp_mem_free` / `LWESP_MEMCPY`
