# LwESP 项目开发规范（C 语言）

> 本文件基于全局 GEMINI.md 改写，专门适配 LwESP（Lightweight ESP-AT parser）C99 嵌入式库项目。

## 一、基础环境

- 使用中文回答，默认环境 Windows 11
- **Shell**：默认使用 PowerShell；执行中文命令前必须 `[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
- **语言标准**：C99（参见 README：Written in C language (C99)）
- **构建系统**：CMake ≥ 3.22，支持 CMakePresets.json
- **代码格式化**：遵循项目根目录 `.clang-format` 配置（基于 LLVM 风格，4 空格缩进，ColumnLimit 120，Attach 大括号）
- **编译选项**：`-Wall -Wextra -Wpedantic`，0 warning 政策
- **全局 Workflow 路径**：`$env:USERPROFILE\.gemini\antigravity\global_workflows\`

## 二、AI 协作红线（HARD-GATE）

**每次调用工具前强制自检，任一项 ❌ 则暂停：**

| # | 自检项 | 规则 |
|:-:|--------|------|
| 1 | **先讨论后动手** | User 还在探讨/分析 → 只给方案，禁止改文件。User 说"执行/开始/直接改"后才动手。例外：User 说"不用讨论/快速修"可跳过 |
| 2 | **禁止越权** | 未经授权禁止：安装依赖、格式化未涉及代码、修改核心配置（`lwesp_opts.h` / `CMakeLists.txt`）。破坏性操作先告知确认 |
| 3 | **流程不跳步** | 前置步骤未完成禁止推进。**完成当前任务后停止，禁止惯性推进** |
| 4 | **项目不越界** | 优先读项目 `.gemini/GEMINI.md` 或 `doc/GEMINI.md`。禁止将其他项目的配置/KI 带入，不确定时先问 |
| 5 | **执行即清理** | 临时脚本用完即删、后台进程完成即 kill |

### 反面案例（每次行动前核对）

#### ❌ 代码简化
- 禁止出现：`TODO`、`...`、`/* 省略 */`、`/* omitted */`、空函数体、placeholder 返回值
- 所有函数必须有完整实现。未想好逻辑时使用 `LWESP_ASSERT(0)` 或 `return lwespERR;` 占位，并附带注释说明原因

#### ❌ 越界执行
- User 说"分析一下" → 只输出分析，不改代码
- User 说"修复这个 Bug" → 先展示 diff，确认后再写入

#### ❌ 惯性推进
- 完成任务 A → 报告结果 → **停止**，等待 User 下一步指令
- 禁止"顺便"优化/重构未提及的文件

### 对话长度控制

当对话超过 **10 轮用户消息**时，在每轮回复末尾追加提醒：

> ⚠️ 当前对话已超过 10 轮，上下文变长会降低规则遵守度。
> 建议：总结当前进度保存为文档，新开对话继续任务。

### 场景 Workflow 强制触发

以下场景 **必须** 在执行前先 `view_file` 对应 Workflow 并遵循其流程：

| 场景识别 | 必须触发的 Workflow |
|---------|-------------------|
| 对话涉及测试、L1/L2/L3 | `/test-flow` |
| 新增/修改调试日志代码（LWESP_DEBUGF） | `/logging-standard` |
| 输出/创建 docs/ 下文档 | `/doc-management` |
| 新建脚本文件或配置文件 | `/project-structure` |

### 输出偏好
- 方案对比用表格，代码改动先展示 diff
- 复杂流程画 Mermaid，工作量用"人天"
- 依赖/兼容问题：先查 CMake 配置约束，列兼容矩阵后再改

## 三、编码规范

### 命名

| 类别 | 规则 | 示例 |
|------|------|------|
| **类型/结构体** | 小写下划线 + `_t` 后缀 | `lwesp_conn_t`, `lwesp_mode_t` |
| **枚举值** | 驼峰式，模块前缀 | `lwespOK`, `lwespERR`, `lwespERRMEM` |
| **函数（公共 API）** | 小写下划线，`lwesp_` 前缀 | `lwesp_init()`, `lwesp_conn_send()` |
| **函数（内部/私有）** | 小写下划线，`lwespi_` 前缀或 `prv_` 前缀 | `lwespi_send_cb()`, `prv_def_callback()` |
| **宏/常量** | 全大写下划线，`LWESP_` 前缀 | `LWESP_CFG_OS`, `LWESP_DBG_LVL_SEVERE` |
| **全局变量** | 小写下划线，模块前缀 | `esp` (全局核心结构体) |
| **文件名** | 小写下划线，`lwesp_` 前缀 | `lwesp_conn.c`, `lwesp_buff.h` |

### 代码风格

- **缩进**：4 空格（禁止 Tab），遵循 `.clang-format` 配置
- **行宽**：≤ 120 字符
- **大括号**：Attach 风格（K&R），与控制语句/函数定义同行
- **返回类型单独一行**：函数定义时返回类型独占一行（`AlwaysBreakAfterReturnType: AllDefinitions`）
  ```c
  lwespr_t
  lwesp_init(lwesp_evt_fn evt_func, const uint32_t blocking) {
      /* ... */
  }
  ```
- **指针对齐**：星号靠左（`char* ptr` 而非 `char *ptr`）
- **控制语句必须有大括号**：即使单行也必须用 `{}`（InsertBraces: true）
- **函数** ≤ 50 行，**文件** ≤ 500 行（特殊模块如 `lwesp_int.c` 除外），**嵌套** ≤ 3 层
- **`#include` 排序**：先系统头文件 `<>`，后项目头文件 `""`

### 注释

- **Doxygen 风格**：公共 API 必须使用 `\brief`、`\param[in/out]`、`\return` 等标签
  ```c
  /**
   * \brief           Brief description
   * \param[in]       name: Parameter description
   * \return          \ref lwespOK on success, member of \ref lwespr_t otherwise
   */
  ```
- **文件头注释**：每个 `.c`/`.h` 文件必须有文件用途注释（`\file` + `\brief`）
- **行内注释**：使用 `/* */` 风格（C99 兼容），`//` 仅在临时调试时允许，提交前必须清除
- **条件编译尾注**：`#endif` 后必须注释对应条件，如 `#endif /* LWESP_CFG_MODE_STATION */`

### 错误处理

- 函数返回值使用 `lwespr_t` 枚举，成功返回 `lwespOK`
- 参数校验使用 `LWESP_ASSERT(expr)` 宏
- 资源分配失败时使用 `goto cleanup` 模式统一释放
- **禁止忽略返回值**：所有可能失败的调用必须检查返回值
- 错误路径必须释放已申请的资源（信号量、消息队列、内存等）

### 内存管理

- 优先使用项目封装的内存管理接口（`lwesp_mem_malloc`/`lwesp_mem_free`/`LWESP_MEMCPY`）
- 禁止直接调用 `malloc`/`free`/`memcpy`
- 动态分配后必须检查 `NULL`
- `free` 后立即置 `NULL`，防止悬挂指针
- 嵌入式环境优先使用静态分配或内存池

### 线程安全

- 访问全局共享数据前必须调用 `lwesp_core_lock()` / `lwesp_core_unlock()`
- 信号量/互斥锁使用 `lwesp_sys_sem_*` / `lwesp_sys_mutex_*` 系列 API
- 消息队列使用 `lwesp_sys_mbox_*` 系列 API
- 禁止在中断上下文中调用阻塞函数

### 条件编译

- 功能开关统一通过 `lwesp_opts.h` 中的 `LWESP_CFG_*` 宏控制
- 条件编译块格式：
  ```c
  #if LWESP_CFG_FEATURE_NAME || __DOXYGEN__
  /* 代码 */
  #endif /* LWESP_CFG_FEATURE_NAME || __DOXYGEN__ */
  ```
- `__DOXYGEN__` 用于确保文档生成时能看到所有 API

## 四、技术栈规范

### 构建系统（CMake）

- 最低版本 CMake 3.22
- 库以 `add_subdirectory()` 方式集成
- 编译定义通过 `target_compile_definitions()` 传递
- 系统移植层通过 `LWESP_SYS_PORT` 变量选择（`win32`/`cmsis_os`/`posix`）
- 配置文件路径通过 `LWESP_OPTS_FILE` 变量指定

### 移植层（System Port）

- 每个平台在 `lwesp/src/system/` 下提供 `lwesp_ll_<platform>.c`
- 必须实现 `lwesp_ll_init()` 函数
- 线程/信号量/互斥锁 API 由 `lwesp_sys_<platform>.c` 提供
- 新增平台时需同步更新 CMake 配置

### 调试与日志

- 使用 `LWESP_DEBUGF()` 宏输出调试信息
- 日志级别组合：`LWESP_DBG_LVL_WARNING` / `LWESP_DBG_LVL_SEVERE` / `LWESP_DBG_LVL_DANGER`
- 日志类型：`LWESP_DBG_TYPE_TRACE` / `LWESP_DBG_TYPE_STATE`
- 模块标识：`LWESP_CFG_DBG_INIT` / `LWESP_CFG_DBG_CONN` 等
- 格式：`"[LWESP MODULE] message\r\n"`
- **严禁在日志中打印敏感信息**（WiFi 密码、密钥等）

### AT 指令交互

- 命令定义使用 `LWESP_CMD_*` 枚举
- 消息变量使用标准宏：`LWESP_MSG_VAR_DEFINE` / `LWESP_MSG_VAR_ALLOC` / `LWESP_MSG_VAR_REF`
- 超时值根据 AT 指令特性合理设置
- 阻塞/非阻塞模式通过 `blocking` 参数控制

### 安全

- WiFi 密码等敏感字段在日志中必须脱敏
- 缓冲区操作必须检查边界，防止溢出
- 外部输入（AT 响应）解析时必须校验长度和格式
- 防止整数溢出：使用 `size_t` 处理长度/大小

## 五、Git

- 格式：`type(scope): subject`（feat/fix/docs/refactor/perf/test/chore）
- scope 使用模块名：`core`/`conn`/`sta`/`ap`/`parser`/`mem`/`build`/`port` 等
- 禁止 git push，大版本/完整需求完成可 commit
- 遵循项目 [C style & coding rules](https://github.com/MaJerle/c-code-style)

## 六、项目结构参考

```
lwesp-develop/
├── CMakeLists.txt          # 顶层 CMake 配置
├── CMakePresets.json        # CMake 预设
├── .clang-format            # 代码格式化配置
├── dev/                     # 开发测试入口 (main.c, lwesp_opts.h)
├── lwesp/
│   ├── CMakeLists.txt       # 库 CMake 配置
│   ├── library.cmake        # 库文件列表
│   └── src/
│       ├── include/         # 公共头文件
│       │   ├── lwesp/       # 库核心头文件
│       │   └── system/      # 系统层头文件
│       ├── lwesp/           # 库核心源文件 (lwesp_*.c)
│       ├── api/             # 高级 API (netconn 等)
│       ├── apps/            # 应用层 (HTTP server, MQTT 等)
│       ├── cli/             # 命令行接口
│       └── system/          # 平台移植层
├── examples/                # 示例代码 (win32/stm32/posix)
├── snippets/                # 代码片段
├── doc/                     # 项目文档
└── docs/                    # ReadTheDocs 文档源
```
